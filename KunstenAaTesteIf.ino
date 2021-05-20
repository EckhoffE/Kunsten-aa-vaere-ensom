/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);
int pro1Lys = A0;

int pro1 = 9;
int pro2 = 10;
int pro3 = 2;
int pro4 = 11;
//String pro1Lys = A0;
//String pro2Lys = A1;
int sistProsjekt;
unsigned long  tid;
unsigned long forrigeTid;
unsigned long pro1Tid = 0;
unsigned long pro2Tid = 0;
unsigned long pro3Tid = 0;
unsigned long pro4Tid = 0;
unsigned long ingenPro = 0;
unsigned long slettetTid = 0;

int prosjekt1;
int prosjekt2;
int prosjekt3;
int prosjekt4;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(pro1,INPUT);
  pinMode(pro2, INPUT);
  pinMode(pro3, INPUT);
  pinMode(pro4, INPUT);
  tid = millis()/1000;
  forrigeTid = 0;
  sistProsjekt = -1;
  Serial.begin(9600);
  // Print a message to the LCD.
  //lcd.print("Prosjekt, 1!");

}

void loop() {
 prosjekt1 = digitalRead(pro1); //flytte deklareringen til oeverst
 prosjekt2 = digitalRead(pro2);
 prosjekt3 = digitalRead(pro3);
 prosjekt4 = digitalRead(pro4);
  tid = millis()/1000;
  if(prosjekt1 == 1){
    if(sistProsjekt != 1){
      sistProsjekt = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Prosjekt 1!    ");
      delay(1000);
    }  
    pro1Tid = tid - pro2Tid - pro3Tid - pro4Tid - ingenPro;
    if((tid-forrigeTid)>0.99){  
        forrigeTid = tid;
        lcd.setCursor(0,1);
      lcd.print(pro1Tid);
      }
  }
  else if(prosjekt2 == 1){
    if(sistProsjekt !=2){
      lcd.clear();
      sistProsjekt = 2;
      lcd.setCursor(0,0);
      lcd.print("Prosjekt 2!    ");
    }
    if((tid-forrigeTid)>0.99){
        forrigeTid = tid;
        lcd.setCursor(0,1);
      lcd.print(pro2Tid);
      }
    pro2Tid = tid - pro1Tid - pro3Tid - pro4Tid - ingenPro;
  }


 else if(prosjekt3 == 1){
    if(sistProsjekt != 3){
      lcd.clear();
      sistProsjekt = 3;
      lcd.setCursor(0,0);
      lcd.print("Prosjekt 3!    ");
    }
    pro3Tid = tid - pro1Tid - pro2Tid - pro4Tid - ingenPro;
       if((tid-forrigeTid)>0.99){
        forrigeTid = tid;
        lcd.setCursor(0,1);
      lcd.print(pro3Tid);
      }
  }
  else if(prosjekt4 == 1){
    if(sistProsjekt != 4){
      lcd.clear();
      sistProsjekt = 4;
      lcd.setCursor(0,0);
      lcd.print("Prosjekt 4!    ");  
      delay(1000); 
    }
    pro4Tid = tid - pro1Tid - pro2Tid - pro3Tid - ingenPro;
     if((tid-forrigeTid)>0.99){
        forrigeTid = tid;
        lcd.setCursor(0,1);
        lcd.print(pro4Tid);
      }
  }
  //else{
  //lcd.clear();
  //lcd.clear();
  //if (prosjekt1 == LOW && prosjekt2 == LOW && prosjekt3 == LOW && prosjekt4 == LOW) 
  else{
    digitalWrite(A0,LOW);
      if(sistProsjekt != 0){
      sistProsjekt = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      delay(1000);
      lcd.print("Ingen prosjekt");//Oppfordring om at man boer velge et prosjekt. "Hva jobber du med nå?:))DDD" Evt faa saa hvor lenge du ikkee har jobbet
      //sjekker opp paa deg om du faktisk jobber?
     }
      ingenPro = tid - pro1Tid - pro2Tid - pro3Tid - pro4Tid;
      if((tid-forrigeTid)>0.99){
        forrigeTid = tid;
        lcd.setCursor(0,1);
      lcd.print(ingenPro);
      }
  }

  //}
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(tid);
  //Serial.println(tid);
}
/*
void klokkeSkriver(unsigned long prosjektTid){
  unsigned long 
  if(prosjektTid/3600>0){
    unsigned long timer = 
  }
}
*/
