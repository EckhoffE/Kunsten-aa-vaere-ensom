
// Biblioteker som brukes:
#include <LiquidCrystal.h> //LCD bibliotek
#include <EEPROM.h> //Bibliotek for lagring til minnet til Arduino
#include <SoftwareSerial.h> //Bibliotek for aa bruke digitale porter til seriell kommunikasjon

SoftwareSerial ESPserial(A4, A5); //Seriell kommunikasjon med WiFi-modul, RX = A4 | TX = A5.
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

//Pinnene prosjektene er koblet til.
int pro1 = A0;
int pro2 = A2;
int pro3 = A1;
int pro4 = A3;

//Pinnene til RGB-LED
int roedRGB = 9;
int groeennRGB = 10;
int blaaRGB = 11; 

//Verdiene som sendes til WiFi-modul.
String verdier;

//Data som mottas fra WiFi-modul.
String inndata;

//Variablene som viser om prosjektene er valgt.
int prosjekt1;
int prosjekt2;
int prosjekt3;
int prosjekt4;

//Forrige prosjekt som har hatt vektloddet i seg. 
int sistProsjekt;

//Tid siden sist tiden ble oppdatert paa LCD.
unsigned long forrigeTid;

//Forrige lagring av tid til minnet.
unsigned long forrigeLagring = 0;

//Samlet tid fra alle prosjektene som har blitt lagret 
unsigned long totalTid;

//Hovedtiden for hele koden. Totaltid og millis() FORKLAR MER UTgangspukt eller no
unsigned long tid;

//Prosjekt1 == roedt.
unsigned long pro1Tid; //Den loepende tiden som vises paa LCD.
unsigned long pro1TidLagret; //Tiden som blir hentet fra minne.
//pro1Tid omregnet til timer, minutter og sekunder.
int prosjekt1Timer; 
int prosjekt1Minutter;
int prosjekt1Sekunder;
//Samme kommentarer gjelder for de under.

//Prosjekt2 == gult 
unsigned long pro2Tid;
unsigned long pro2TidLagret;
int prosjekt2Timer;
int prosjekt2Minutter;
int prosjekt2Sekunder;

//Prosjekt3 == groent
unsigned long pro3Tid;
unsigned long pro3TidLagret;
int prosjekt3Timer;
int prosjekt3Minutter;
int prosjekt3Sekunder;

//Prosjekt4 == blaatt
unsigned long pro4Tid;
unsigned long pro4TidLagret;
unsigned long pro4FaktiskTid;
int prosjekt4Timer;
int prosjekt4Minutter;
int prosjekt4Sekunder;

//Annet prosjekt
unsigned long pro0Tid;
unsigned long pro0TidLagret;
unsigned long pro0FaktiskTid;
int prosjekt0Timer;
int prosjekt0Minutter;
int prosjekt0Sekunder;

void setup() {
  Serial.begin(9600); //Starter opp seriell kommunikasjon med PC for debugging.
  ESPserial.begin(9600); //Starter opp seriell kommunikasjon med WiFi-modul.
  lcd.begin(16, 2); //Starter opp LCD-panelet.
  lcd.setCursor(0, 0);
  lcd.print("Starter opp...");
  pinMode(pro1, INPUT); //Prosjekt1
  pinMode(pro2, INPUT); //Prosjekt2
  pinMode(pro3, INPUT); //Prosjekt3 
  pinMode(pro4, INPUT); //Prosjekt4
  pinMode(roedRGB, OUTPUT);
  pinMode(groeennRGB, OUTPUT);
  pinMode(blaaRGB, OUTPUT);

  //Henter opp lagret tid fra minnet paa Arduinoen
  pro0TidLagret = HentTid(0);
  pro1TidLagret = HentTid(1);
  pro2TidLagret = HentTid(2);
  pro3TidLagret = HentTid(3);
  pro4TidLagret = HentTid(4);
  //Adder sammen total lagret tid
  totalTid = pro0TidLagret + pro1TidLagret + pro2TidLagret + pro3TidLagret + pro4TidLagret;
  
  //Setter hentet tid til prosjektets tid.
  pro0Tid = pro0TidLagret;
  pro1Tid = pro1TidLagret;
  pro2Tid = pro2TidLagret;
  pro3Tid = pro3TidLagret;
  pro4Tid = pro4TidLagret;
  
  //tid siden sist oppdatert tidsvisning 
  forrigeTid = 0; 

  //Hvilket prosjekt som sist har vaert i bruk. Bruker -1 fordi da vil den oppdatere seg fra oppstart.
  sistProsjekt = -1;

  //Skriver ut en visuell progresjonsbar ved oppstart for aa indikere naar systmet er klart for bruk.
  for (int i = 0; i<16;i++){
     lcd.setCursor(i, 1);
     lcd.print("|");
     delay(625); //Totalt 10 sekunder for aa gi WiFi-modul tid for aa starte og koble seg opp.
  }
}


void loop() {
  //Leser hvilket prosjekt som er valgt. 
  //Hvis ingen prosjekter er valgt, viser den tiden for "annet"-prosjekt.
  prosjekt1 = digitalRead(pro1);
  prosjekt2 = digitalRead(pro2);
  prosjekt3 = digitalRead(pro3);
  prosjekt4 = digitalRead(pro4);
  
  //Tiden blir oppdatert underveis. totalTid er tid hentet fra minnet 
  //og millis() teller millisekunder fra oppstart av arduino
  tid = totalTid + (millis() / 1000); //Deler paa 1000 for aa faa sekunder

  //Nullstiller variablene for kommunikasjon.
  verdier = "";
  inndata = "";
  
  //Hvis WiFi-modul er tilgjengelig for aa skrive seriell kommunikasjon
  if ( ESPserial.available() ) {
    inndata = ESPserial.readString(); //Leser Stringen WiFi-modulen sender.
    inndata.trim(); //Fjerner mellomrom foran og bak bokstavene.
    Serial.print(inndata); //Brukes til debugging.
    
    //Fargen paa RGB endrer seg avhengig av data fra Google Regneark som er hva partner jobber med
    if(inndata.equals("roed")){
      ledRGB(255,0,0);
    }
    else if (inndata.equals("gul")){
      ledRGB(0,255,255);
    }
    else if (inndata.equals("groeenn")){
      ledRGB(0,255,0);
    }
    else if (inndata.equals("blaa")){
      ledRGB(0,0,255);
    }

    else if (inndata.equals("offline")){
      ledRGB(0,0,0);
    }
  }
  //Lagrer tiden til minnet hvert 10. sekund.
  if ((tid - forrigeLagring) > 10) {
    forrigeLagring = tid;
    //Lagrer tiden for hvert prosjekt til minnet. 
    LagreTid(1, prosjekt1Timer, prosjekt1Minutter, prosjekt1Sekunder);
    LagreTid(2, prosjekt2Timer, prosjekt2Minutter, prosjekt2Sekunder);
    LagreTid(3, prosjekt3Timer, prosjekt3Minutter, prosjekt3Sekunder);
    LagreTid(4, prosjekt4Timer, prosjekt4Minutter, prosjekt4Sekunder);
    LagreTid(0, prosjekt0Timer, prosjekt0Minutter, prosjekt0Sekunder);
    
    //Sender verdiene til WiFi-modul
    //"<" og ">" gjoer det lettere aa se om alle sifferne har blitt sendt paa riktig maate.
    verdier = "<"+ String(riktigFormatTid(prosjekt1Timer)) + "," + String(riktigFormatTid(prosjekt1Minutter)) + ",";
    verdier += String(riktigFormatTid(prosjekt2Timer)) + "," + String(riktigFormatTid(prosjekt2Minutter)) + ",";
    verdier += String(riktigFormatTid(prosjekt3Timer)) + "," + String(riktigFormatTid(prosjekt3Minutter)) + ",";
    verdier += String(riktigFormatTid(prosjekt4Timer)) + "," + String(riktigFormatTid(prosjekt4Minutter))+ ">";
    
    //Brukes til aa debugge og se hva som blir skrevet til minnet. 
    skrivUtInfo(0);
    skrivUtInfo(1);
    skrivUtInfo(2);
    skrivUtInfo(3);
    skrivUtInfo(4);
    Serial.println(verdier); //Brukes til aa debugge. 
    
    //Sender verdiene til WiFi-modulen.
    ESPserial.print(verdier);
  }
 
  //Registerer ulike prosjekter.
  if (prosjekt1 == 1) { //Hvis prosjekt1 er valgt.
    //Finner hvor lang tid prosjekt1 har vaert valgt ved trekke de andre prosjektene fra hovedtiden. 
    pro1Tid = tid - pro2Tid - pro3Tid - pro4Tid - pro0Tid; 
    //Lagrer i egne variablerer hvor lenge prosjektet har vaert valgt i timer, minutter og sekunder.
    prosjekt1Timer = OppdatereTimer(pro1Tid);
    prosjekt1Minutter = OppdatereMinutter(pro1Tid);
    prosjekt1Sekunder = OppdatereSekunder(pro1Tid);
    //Gjoer om tiden til en String som skrives ut paa LCD-panelet.
    String tekst1 = skrivTid(prosjekt1Timer, prosjekt1Minutter, prosjekt1Sekunder);
    //Se Prosjekt()
    Prosjekt(1, tekst1, "Prosjekt roed");   
  }
  else if (prosjekt2 == 1) {
    pro2Tid = tid - pro1Tid - pro3Tid - pro4Tid - pro0Tid;
    prosjekt2Timer = OppdatereTimer(pro2Tid);
    prosjekt2Minutter = OppdatereMinutter(pro2Tid);
    prosjekt2Sekunder = OppdatereSekunder(pro2Tid);
    String tekst2 = skrivTid(prosjekt2Timer, prosjekt2Minutter, prosjekt2Sekunder);
    Prosjekt(2, tekst2, "Prosjekt gul");
  }
  else if (prosjekt3 == 1) {
    pro3Tid = tid - pro1Tid - pro2Tid - pro4Tid - pro0Tid;
    String tekst3 = skrivTid(prosjekt3Timer, prosjekt3Minutter, prosjekt3Sekunder);
    prosjekt3Timer = OppdatereTimer(pro3Tid);
    prosjekt3Minutter = OppdatereMinutter(pro3Tid);
    prosjekt3Sekunder = OppdatereSekunder(pro3Tid);
    Prosjekt(3, tekst3, "Prosjekt groeenn");
  }
  else if (prosjekt4 == 1) {
    pro4Tid = tid - pro1Tid - pro2Tid - pro3Tid - pro0Tid;
    prosjekt4Timer = OppdatereTimer(pro4Tid);
    prosjekt4Minutter = OppdatereMinutter(pro4Tid);
    prosjekt4Sekunder = OppdatereSekunder(pro4Tid);
    String tekst4 = skrivTid(prosjekt4Timer, prosjekt4Minutter, prosjekt4Sekunder);
    Prosjekt(4, tekst4, "Prosjekt blaa");
  }
  //Annet prosjekt
  else {
    pro0Tid = tid - pro1Tid - pro2Tid - pro3Tid - pro4Tid;
    prosjekt0Timer = OppdatereTimer(pro0Tid);
    prosjekt0Minutter = OppdatereMinutter(pro0Tid);
    prosjekt0Sekunder = OppdatereSekunder(pro0Tid);
    String tekst0 = skrivTid(prosjekt0Timer, prosjekt0Minutter, prosjekt0Sekunder);
    Prosjekt(0, tekst0, "Annet");
  }
}


void Prosjekt(int prosjektNr, String printTid, String printTekst) {
  if (sistProsjekt != prosjektNr) {//Hvis det er et nytt prosjekt saa oppdateres teksten LCD-panelet.
    sistProsjekt = prosjektNr;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(printTekst);
  }
  if ((tid - forrigeTid) > 0.99) { //Oppdaterer tidsvisningen en gang hvert sekund.
    forrigeTid = tid;
    lcd.setCursor(0,1);
    lcd.print(printTid);
  }
}
//Gir antall timer som prosjektet har vaert paa.
int OppdatereTimer(unsigned long prosjektTid) {
  return prosjektTid / 3600;
}
//Gir antall minutter som prosjektet har vaert paa.
//Utnytter at int ikke lagrer desimaler. 
int OppdatereMinutter(unsigned long prosjektTid) {
  int prosjektTimer = prosjektTid / 3600; //Gir antall timer
  int prosjektMinutter = (prosjektTid - (prosjektTimer * 3600)) / 60;
  return prosjektMinutter;
}
//Gir antall sekundner som prosjektet har vaert paa.
int OppdatereSekunder(unsigned long prosjektTid) {
  int prosjektTimer = prosjektTid / 3600; //Gir antall timer
  int prosjektMinutter = (prosjektTid - (prosjektTimer * 3600)) / 60; //Gir antall minutter
  int prosjektSekunder = prosjektTid - ((prosjektTimer * 3600) + (prosjektMinutter * 60)); //Gir antall sekunder
  return prosjektSekunder;
}

//Lagrer antall timer, minutter og sekunder prosjektet har vaert paa i minnet paa Arduinoen.

void LagreTid(int prosjektNr, int prosjektTimer, int prosjektMinutter, int prosjektSekunder) {
  //Hvert prosjekt faar sin egen plass i minnet fra 10*prosjektNr til 10*prosjektNr+2 
  int adresse = prosjektNr * 10;
  //Med EEPROM.update() vil programmet kun skrive til minnet hvis det er en ny verdi aa skrive,
  //dette sparer minnet for slitasje. 
  EEPROM.update(adresse, prosjektTimer);
  EEPROM.update(adresse + 1, prosjektMinutter);
  EEPROM.update(adresse + 2, prosjektSekunder);
}

//Henter tiden lagret i minnet.
unsigned long HentTid(int prosjektNr) {
  int adresse = prosjektNr * 10;
  int timer = EEPROM.read(adresse);
  //Minnet har opprinnlig verdien 255, og 255 er maks stoerrelse paa tallet. Hindrer at den kraesjer.
  //Grunnet at minnet bestaar av 1 byte.
  if (timer == 255) {
    timer = 0;
  }
  int minutter = EEPROM.read(adresse + 1);
  if (minutter == 255) {
    minutter = 0;
  }
  int sekunder = EEPROM.read(adresse + 2);
  if (sekunder == 255) {
    sekunder = 0;
  }
  //Gjoer om timene og minutter til sekunder. Retunerer antall sekunder.
  unsigned long antallSek = timer * 3600 + minutter * 60 + sekunder;
  return antallSek;
}

//For debugging og sjekke hva som har blitt skrevet i minnet.
void skrivUtInfo(int prosjektNr) {
  int adresse = prosjektNr * 10;
  Serial.print("Prosjekt ");
  Serial.print(prosjektNr);
  Serial.print( " :");
  Serial.print(EEPROM.read(adresse));
  Serial.print(" Min: ");
  Serial.print(EEPROM.read(adresse + 1));
  Serial.print(" Sek: ");
  Serial.print(EEPROM.read(adresse + 2));
  Serial.println();
}

//Returnerer paa String-format en representasjon av tiden.
String skrivTid(int t, int m, int s){
    String timer = riktigFormatTid(t);
    String minutter = riktigFormatTid(m);
    String sekunder = riktigFormatTid(s);
    String retur = timer + ":" + minutter + ":" + sekunder;
    return retur;
}

//Soerger for at det kommer en "0" foran tall som er mindre enn 10. 
String riktigFormatTid(int tid){
    String retur;
    if(tid<10){
        retur = "0"+String(tid);
    }
    else{
        retur = tid;
    }
    return retur;
}

//Styrer LED-RGB.
void ledRGB(int roedRGBverdi, int groeennRGBverdi, int blaaRGBverdi){
  analogWrite(roedRGB, roedRGBverdi);
  analogWrite(groeennRGB, groeennRGBverdi);
  analogWrite(blaaRGB, blaaRGBverdi);
}
