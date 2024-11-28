#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <FastLED.h>

void initLEDs();
void SetColor(int I = -1);
void LowBattery();

#define NUM_LEDS 30
#define DATA_PIN 8

extern CRGB leds[NUM_LEDS];

#endif // LEDCONTROL_H
