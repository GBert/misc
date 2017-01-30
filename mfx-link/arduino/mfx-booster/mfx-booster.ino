/*
 * DCC Booster using the 43 amp H-Bridge from eBay & Amazon
 * by D.Bodnar
 */

#include<EEPROM.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int LED = 13; // LED to blink when DCC packets are sent in loop
int PowerOn = 3; // pin to turn booster on/off
int Pot1 = 1;
float PotReading = 0;
int CurrentPin = 0;
int CurrentPinReading = 0;
int SetPotFlag = 0;

float version = 1.4;

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
LiquidCrystal_I2C	lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

unsigned long time;
unsigned long now;
long cAverage = 0;
int avgTimes = 400;
int lastAverage = 0;
int resetFlag = 0;
float percentage = 0;
int temp1 = 0;
void setup() {
  delay(500);
  pinMode(LED, OUTPUT);
  pinMode(PowerOn, OUTPUT );
  lcd.begin (16, 2); //  LCD is 16 characters x 2 lines
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);  // Switch on the backlight
  lcd.home (); // go home
  Serial.begin (115200);
  lcd.setCursor(0, 0);
  lcd.print("DCC Booster");
  lcd.setCursor(0, 1);
  lcd.print("2-11-16 - v");
  lcd.print(version, 1);
  Serial.print("2-11-2016  version  ");
  Serial.println(version, 1);
  delay(1000);
  lcd.clear();
  turnPowerOn();
  delay(500);
  now = millis();
}

// NOTE:  about 0.014 amps / digit of reading with 10K resistor on sense line
void loop() {
  PotReading = analogRead(Pot1);
  PotReading = PotReading / 100;
  lcd.setCursor(12, 0);
  lcd.print(PotReading, 1);
  lcd.print("   ");
  lcd.setCursor(7, 0);
  lcd.print("MaxA=");
  showPercentage();
  resetFlag++;
  if (resetFlag >= 100) {
    resetFlag = 0;
  }
  cAverage = 0;
  for (int xx = 0; xx < avgTimes ; xx++) {
    CurrentPinReading = analogRead(CurrentPin);
    if (CurrentPinReading >= 1000) {
      Serial.println("STOPPPPPPPPPPPPPPPPPPPPPPING");
      turnPowerOff();
    }
    cAverage = cAverage + CurrentPinReading;
  }
  CurrentPinReading = cAverage / avgTimes;
  if (CurrentPinReading != lastAverage) {
    if (millis() - now >= 450) {  // only update LCD every 1/2 second to limit flicker
      lcd.setCursor(0, 0);
      lcd.print("C=");
      lcd.print(CurrentPinReading);
      lcd.print("  ");
    }
    turnPowerOn();
  }
  lastAverage = CurrentPinReading; // keep for compare & print
}  //END LOOP

void showPercentage()
{
  percentage = (CurrentPinReading * 0.0105) / PotReading; // was 0.014
  percentage = percentage * 100;
  if (millis() - now >= 500)   // only update LCD every 1/2 second to limit flicker
  {
    lcd.setCursor(9, 1);
    lcd.print(percentage,1);
    lcd.print("%  ");
    now = millis();
  }
  if (percentage > 100) turnPowerOff();
}

void turnPowerOff() {
  digitalWrite(PowerOn, LOW);
  lcd.setCursor(0, 1);
  lcd.print("PWR OFF-2 sec");
  delay(2000);
  turnPowerOn();
  lcd.setCursor(0, 1);
  lcd.print("               ");
}

void turnPowerOn() {
  digitalWrite(PowerOn, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("PWR On");
}
