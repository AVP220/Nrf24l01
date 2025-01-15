#include "LEDControl.h"
#include "DeviceStatus.h"

CRGB leds[NUM_LEDS];

void initLEDs() { 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS); 
  FastLED.setBrightness(100);
}

void SetColor(int I)
{
  if (I == -1)
  {
    for (int i = 0; i < 30; i++)
    {
      if (MyData[i].connect)
      {
        if (MyData[i].flag)
        {
          if (MyData[i].battery == 254)
          {
            Serial.println("SetDarkGreen");
            MyData[i].myColor[0] = 255; //{255,0,0}; //DarkGreen
            MyData[i].myColor[1] = 75;
            MyData[i].myColor[2] = 0;
          }
          else
          {
            Serial.println("setGreen");
            MyData[i].myColor[0] = 255; //{0,255,0}; //green
            MyData[i].myColor[1] = 0;
            MyData[i].myColor[2] = 0;
          }
        }
        else
        {
          if (MyData[i].battery == 254)
          {
            Serial.println("SetDarkRed");
            MyData[i].myColor[0] = 75; //{255,0,0}; //SetDarkRed
            MyData[i].myColor[1] = 255;
            MyData[i].myColor[2] = 0;
          }
          else
          {
            Serial.println("SetRED");
            MyData[i].myColor[0] = 0; //{255,0,0}; //red
            MyData[i].myColor[1] = 255;
            MyData[i].myColor[2] = 0;
          }
        }
      }
      else
      {
        Serial.println("SetBLUE");
        MyData[i].myColor[0] = 0; //{0,0,255}; // blue
        MyData[i].myColor[1] = 0;
        MyData[i].myColor[2] = 255;
      }

      // update color\TrueColor
      leds[i] = CRGB(MyData[i].myColor[0], MyData[i].myColor[1],
                     MyData[i].myColor[2]); // установка правильных цветов
      FastLED.show();
    }
  }
  else
  {
    if (MyData[I].connect)
    {
      if (MyData[I].flag)
      {
        if (MyData[I].battery == 254)
        {
          Serial.println("SetDarkGreen");
          MyData[I].myColor[0] = 255; //{255,0,0}; //red
          MyData[I].myColor[1] = 75;
          MyData[I].myColor[2] = 0;
        }
        else
        {
          Serial.println("setGreen");
          MyData[I].myColor[0] = 255; //{0,255,0}; //green
          MyData[I].myColor[1] = 0;
          MyData[I].myColor[2] = 0;
        }
      }
      else
      {
        if (MyData[I].battery == 100)
        {
          Serial.println("SetDarkRed");
          MyData[I].myColor[0] = 75; //{255,0,0}; //red
          MyData[I].myColor[1] = 255;
          MyData[I].myColor[2] = 0;
        }
        else
        {
          Serial.println("SetRED");
          MyData[I].myColor[0] = 0; //{255,0,0}; //red
          MyData[I].myColor[1] = 255;
          MyData[I].myColor[2] = 0;
        }
      }
    }
    else
    {
      Serial.println("SetBLUE");
      MyData[I].myColor[0] = 0; //{0,0,255}; // blue
      MyData[I].myColor[1] = 0;
      MyData[I].myColor[2] = 255;
    }

    // update color\TrueColor
    leds[I] = CRGB(MyData[I].myColor[0], MyData[I].myColor[1],
                   MyData[I].myColor[2]); // установка правильных цветов
    FastLED.show();
  }
}

void LowBattery()
{
  for (int turnover = 0; turnover < 3; turnover++)
  {
    delay(100);
    SetColor();
    delay(100);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(255, 0, 0); // Red
    }
  }
}