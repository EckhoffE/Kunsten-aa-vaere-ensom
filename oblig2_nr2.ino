#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int stst = 8; //startstopp
const int reset = 9;
int ststTil; //Tilstand
int resetTil;

int forrigeReset;

int minu = 0;
int sek = 0;
int tsek = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 10);
  pinMode(stst, INPUT);
  pinMode(reset, INPUT);
  lcd.setCursor(2, 0);
  lcd.print("STOPPEKLOKKE");
  lcd.setCursor(0, 1);
  lcd.print("TID:");
  lcd.setCursor(4,1);
}

void loop() {
  lcd.setCursor(2,0); 
  lcd.print("STOPPEKLOKKE");  
  lcd.setCursor(0,1);  
  lcd.print("TID:"); 
  lcd.print("   ");
  lcd.print(minu); 
  lcd.print(":"); 
  lcd.print(sek);
  lcd.print(":"); 
  lcd.print(tsek);
  
  ststTil = digitalRead(stst); 
 if(ststTil == HIGH){
  stoppeklokke();  
 }
}
//metode for å starte stoppeklokken 
void stoppeklokke() {
  lcd.setCursor(2,0); 
  lcd.print("STOPPEKLOKKE");  
  lcd.setCursor(0,1);   
  lcd.print("TID:");
  lcd.print("   ");      
  lcd.print(minu);         
  lcd.print(":"); 
  lcd.print(sek);
  lcd.print(":");
  lcd.print(tsek);
  delay(80);
  tsek = tsek+10;

   
  if (tsek == 100){
      tsek = 0;
      sek++;
  }if (sek == 60) {
        sek = 0;
        minu++;  
  }
     
 ststTil = digitalRead(stst);  
 if(ststTil == HIGH){
  stoppStoppeklokke();   
 }else {
    stoppeklokke();    
  }
}

// metode som stopper tiden midlertidig
void stoppStoppeklokke() {
  lcd.setCursor(2,0); 
  lcd.print("STOPPEKLOKKE");  
  lcd.setCursor(0,1); 
  lcd.print("TID:"); 
  lcd.print("   ");
  lcd.setCursor(1,4);
  lcd.print(minu); 
  lcd.print(":"); 
  lcd.print(sek);  
  lcd.print(":");
  lcd.print(tsek);   

   ststTil = digitalRead(stst);   
 if(ststTil == HIGH){
  stoppeklokke();    
 } 
 
 resetTil = digitalRead(reset);   
 if(resetTil == HIGH){
   stoppeklokkeReset();   
  }
 if(resetTil == LOW){
  stoppStoppeklokke();    
 }
}
//metode som restarter tiden
void stoppeklokkeReset() {
 lcd.setCursor(2,0); 
 lcd.print("STOPPEKLOKKE");  
 lcd.setCursor(0, 1);
  lcd.print("TID:");
  lcd.setCursor(4,1);
 lcd.clear(); 
 minu=0;    
 sek=0;
 tsek = 0;    
 return;  
 }
 
