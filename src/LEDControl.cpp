#include "LEDControl.h"
#include "DeviceStatus.h"

CRGB leds[NUM_LEDS];

void initLEDs() { FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); }

void SetColor() {
  // Serial.println("SetColor();");
  for (int i = 0; i < 30; i++) {
    if (MyData[i].connect) {
      if (MyData[i].flag) {
        Serial.println("setGreen");
        MyData[i].myColor[0] = 255; //{0,255,0}; //green
        MyData[i].myColor[1] = 0;
        MyData[i].myColor[2] = 0;
      } else {
        Serial.println("SetRED");
        MyData[i].myColor[0] = 0; //{255,0,0}; //red
        MyData[i].myColor[1] = 255;
        MyData[i].myColor[2] = 0;
      }
    } else {
      Serial.println("SetBLUE");
      MyData[i].myColor[0] = 0; //{0,0,255}; // blue
      MyData[i].myColor[1] = 0;
      MyData[i].myColor[2] = 255;
    }
    // Serial.print("state: ");
    // Serial.println(MyData[i].battery);
    // if (MyData[i].battery == 1111) {
    //   Serial.println("LowBatteryOnFlag");
    //   for (int j = 200; j > 100; j - 5) {
    //     Serial.println("j--");
    //     // реализация идеии с изменением яркости
    //     unsigned long startTimeBattery = millis();
    //     if (millis() - startTimeBattery >= 50) {
    //       startTimeBattery = millis();
    //       leds[i].nscale8_video(j);
    //       FastLED.show();
    //     }
    //     if (j == 100)
    //       j = 200;
    //   }
    // }
  }
}

void TrueColor() {
  // Serial.println("TrueColor");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(MyData[i].myColor[0], MyData[i].myColor[1],
                   MyData[i].myColor[2]); // установка правильных цветов
    // Serial.println("sentColor");
    FastLED.show();
  }
}

void LowBattery() {
  for (int turnover = 0; turnover < 3; turnover++) {
    delay(100);
    TrueColor();
    delay(100);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 0, 0); // Red
    }
  }
}