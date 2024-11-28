#include <Arduino.h>
#include "DeviceStatus.h"
#include "RadioComm.h"
#include "LEDControl.h"
#include "ButtonControl.h"

#define VOLTAGE_TRIGGER_BATTERY 10
#define BATTERY_PIN A0

unsigned long startTime1;
unsigned long startTimeCheck;

void setup() {
  Serial.begin(9600);
  initLEDs();
  initButtons();
  radio_init();

  for (int i = 0; i < 30; i++) {
    MyData[i].address = addressesLong[i];
    ping(i);
  }

  SetColor();

  startTime1 = millis();
  startTimeCheck = millis();
}

void loop() {
  // if (millis() - startTimeCheck >= 5000) {
  //   startTimeCheck = millis();
  //   SetColor();
  // }
  if (millis() - startTime1 >= 60000) {
    startTime1 = millis();
    for (int i = 0; i < 30; i++) {
      ping(i);
    }
    SetColor();
    if (analogRead(BATTERY_PIN) < VOLTAGE_TRIGGER_BATTERY) {
      LowBattery();
    }
  }
  updateButtons();
}
