/*
  Denne koden er basert paa "Storage B" sin kode.
  WiFi-modulen tar i mot verdiene med antall timer og minutter i en streng og sender det videre til
  Google Apps Script. Skriptet returnerer tilbake informasjon om hvilket prosjekt partneren jobber paa.
*/

// Example Arduino/ESP8266 code to upload data to Google Sheets
// Follow setup instructions found here:
// https://github.com/StorageB/Google-Sheets-Logging
// contact: storageunitb@gmail.com
//
// This example uses the HTTPSRedirect library by Sujay Phadke
// https://github.com/electronicsguy/ESP8266


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

//LED-statuslys rett fra pin paa WiFi-modulen.
//Lyser naar den har tilkobling til WiFi.
int LED = 2; 

// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbwHuTM7s4ZBUtM4zXe5BmQS_F_965mU4D_sIHFU0Zk45iBvo_KtTUjOti8mSL5ebIoimw";

//Det som sendes til Google Apps Script
// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Ark1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec?cal";
HTTPSRedirect* client = nullptr;

void setup() {
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW); //LED-statuslys er av til og starte med.
  Serial.begin(9600);        
  delay(10);
  Serial.println('\n');
  //Mulige nettverk wifi-modulen kan koble seg paa.
  wifiMulti.addAP("hjemNett", "eksempel!");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("Jakob iphone", "mellomrom");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
  
  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
  }
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
       flag = true;
       Serial.println("Connected");
       digitalWrite(,HIGH);
       break;
    }
    else
      Serial.println("Connection fai. Retrying...");
      digitalWrite(LED,LOW); //Hvis det blir feil saa blir LED skrudd av.
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
}


void loop() {
 String IncomingString=""; //String som blir sendt fra Arduinoen
 String ren =""; //Strippet String uten "<" og ">".
 boolean StringReady = false; //Flagg for om det har blitt mottatt en String
 
 while (Serial.available()){ //Leser String fra Arduinoen
   IncomingString = Serial.readString();
   StringReady = true; //Markerer at Stringen er klar for aa brukes.
  }
 
  if (StringReady){
    Serial.println(IncomingString);//Skjules med mindre man driver debugging
    ren = IncomingString.substring(1,24);//Stripper teksten ved aa fjerne "<" og ">"
    static bool flag = false;
    if (!flag){
        client = new HTTPSRedirect(httpsPort);
        client->setInsecure();
        flag = true;
        client->setPrintResponseBody(true);
        client->setContentTypeHeader("application/json");
      }
    if (client != nullptr){
      if (!client->connected()){
        client->connect(host, httpsPort);
      }
  }
  else{
    Serial.println("Error creating client object!");
  }

  payload = payload_base + "\""+ ren + "\"}";
  
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
    digitalWrite(LED,HIGH); //Saa lenge WiFi-modulen faar publisert dataen lyser LED-status.
  }
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
    digitalWrite(LED,LOW); //Hvis det blir feil saa skru LED-status seg av.
  }
  // a delay of several seconds is required before publishing again    
  delay(8000);
}
delay(1000);
}
