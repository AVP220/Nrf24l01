#include <SPI.h>
#include <RF24.h>
#include "nRF24L01.h"     // библиотека радиомодуля
#include <GyverButton.h>
#include <FastLED.h>
#include <Arduino.h> 

char addressesLong[30][6] = {"00001", "00002", "00003","00004", "00005", "00006","00007", "00008", "00009","00010", "00011", "00012","00013", "00014", "00015","00016", "00017", "00018","00019", "00020", "00021","00022", "00023", "00024","00025", "00026", "00027","00028", "00029", "00030"};

struct DeviseStatus {
  bool connect;
  bool battery;
  bool flag;
  int ledsNum;
  int myColor[3];
  char *adresse;
};

const int buttonPins[] = {2, 3, 4, 5, 6, 7, A1, A2, A3, A4};

#define VOLTAGE_TRIGGER_BATTERY 3.3 //вольтж акумулятора ниже которого срабатывает индекация
#define BATTERY_PIN A0 //пин для измирения наприжения акумулятора


#define NUM_LEDS 30 // Количество светодиодов в ленте
#define DATA_PIN 8 // Пин для подключения светодиодной ленты 

DeviseStatus MyData[30];
RF24 radio(9, 10); // CE на D9, CSN на D10
CRGB leds[NUM_LEDS];



typedef struct {
    GButton button; // Объект GButton
    bool state;
} ButtonStruct;
// Количество кнопок, равное размеру массива buttonPins
const int NUM_BUTTONS = sizeof(buttonPins) / sizeof(buttonPins[0]);
// Создаем массив структур ButtonStruct
ButtonStruct buttons[NUM_BUTTONS] = { {GButton(0)} }; // Инициализируем с временным значением

unsigned long startTime1;
unsigned long startTimeCheck;







void radio_init(){
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(6);     //размер пакета, в байтах
  // radio.openReadingPipe(1, address[0]);     //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)
  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}


void ping(int deviceNum) {
  radio.openWritingPipe(reinterpret_cast<const uint8_t*>(MyData[deviceNum].adresse));
  radio.stopListening();

  const int checkCommand = 111;
  bool sendResult = radio.write(&checkCommand, sizeof(checkCommand));

  if (sendResult) {
    // Устанавливаем время начала ожидания
    unsigned long startTime = millis();
    bool timeout = false;

    // Ждем ответ в течение таймаута
    while (!radio.isAckPayloadAvailable()) {
      if (millis() - startTime > 200) { // Таймаут 200 мс
        timeout = true;
        break;
      }
    }

    if (!timeout && radio.isAckPayloadAvailable()) {
      //идея передать масив с двумя значениями, первое - значение акума, второе - значение флага
      int answer[2];
      radio.read(&answer, sizeof(answer));

      MyData[deviceNum].connect = true;
      MyData[deviceNum].battery = answer[0];
      MyData[deviceNum].flag = answer[1];
    } else {
      // Ответ не получен, отмечаем устройство как не подключенное
      MyData[deviceNum].connect = false;
      MyData[deviceNum].battery = 0;
    }
  }
  radio.startListening();
}

void Write(int myGroup, bool CommandToSent){
  for(int i = 0; i < 3; i++){
  radio.openWritingPipe(reinterpret_cast<const uint8_t*>(MyData[i + (myGroup*3)].adresse));
  radio.stopListening(); 
//и поидеии где-то здесь должен быть таймаут как в пинге чтоб отсеивать не подключеные
  bool success = radio.write(&CommandToSent, sizeof(CommandToSent));//можно попробывать изминить bool на int и присылать так: -1 - опущен, 1 - поднят, 0 неизвестно, поскольу погут быть потенциальные проблемы с индекацией в SetColor()
  bool returnFlag;
  if (success && radio.isAckPayloadAvailable()) {
    radio.read(&returnFlag, sizeof(returnFlag));
    MyData[i].flag = returnFlag;
    }else{
      ping(i);
    }
  }
}






void SetColor(){
  for(int i = 0; i < 30; i++){
    if(MyData[i].connect){
      if(MyData[i].flag){
        MyData[i].myColor[0] = 0; //{0,255,0}; //green
        MyData[i].myColor[1] = 255;
        MyData[i].myColor[2] = 0;
        if(MyData[i].battery){
          for(int i = 200; i < 100; i-5){
            //реализация идеии с изменением яркости
            unsigned long startTimeBattery = millis();
            if (millis() - startTimeBattery >= 50) {  
              startTimeBattery = millis();
              FastLED.setBrightness(i); 
            }
            if(i==100)i = 200;
          }
        }
      }else{
        MyData[i].myColor[0] = 255; //{255,0,0}; //red
        MyData[i].myColor[1] = 0;
        MyData[i].myColor[2] = 0;
      }
    }else{
      MyData[i].myColor[0] = 0; //{0,0,255}; // blue
      MyData[i].myColor[1] = 0;
      MyData[i].myColor[2] = 255;
    }
  }
}


void TrueColor(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(MyData[i].myColor[0], MyData[i].myColor[1], MyData[i].myColor[2]); // установка правильных цветов
    FastLED.show();
  }
}





void sys_init(){
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i] = { GButton(buttonPins[i]) }; // Инициализация кнопки с соответствующим пином
  }
  for(int i = 0; i < 30; i++){
    MyData[i].adresse = addressesLong[i];
  }
  unsigned long startTime1 = millis();
  unsigned long startTimeCheck = millis();
}





void setup() {
  Serial.begin(9600);
  radio_init();
  sys_init();
  for(int i = 0; i < 30; i++){
    ping(i);
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






void loop() {
  if (millis() - startTime1 >= 60000) {  
    startTime1 = millis();
    for(int i = 0; i < 30; i++){
      ping(i);
    }
    if(analogRead(BATTERY_PIN) < VOLTAGE_TRIGGER_BATTERY){
      LowBattery();
  }

//millis 200ms 
  if (millis() - startTimeCheck >= 200) {  
    startTimeCheck = millis(); 
    for (int i = 0; i < NUM_BUTTONS; i++) {
      buttons[i].button.tick(); // Обновляем состояние кнопки
      if (buttons[i].button.isHold()){
        Write(i,1);
        }else if(!buttons[i].button.isHold()){ //зделал чуть проще если не прокатит то передалаю 
           Write(i,0);
        }
      }
    }
  }
}






