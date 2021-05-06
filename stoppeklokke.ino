#include <LiquidCrystal.h>

int knapp = 2;
int tidTaker = 0;
long startTid;

LiquidCrystal lcd (7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(knapp, INPUT_PULLUP);
  lcd.print("Start/Stopp");
}

void loop() {
  lcd.setCursor(0, 1);
  if (digitalRead(knapp) == LOW) {
    startTid = millis();
    tidTaker++;
    delay(400);
  }
  if (tidTaker == 1) {
    lcd.setCursor(0, 1);
    lcd.print((millis() - startTid) / 1000.0);
  }
  if (tidTaker > 1) {
    delay(2000);
    tidTaker = 0;
    lcd.clear();
    lcd.print("Start/Stopp");
  }
}
