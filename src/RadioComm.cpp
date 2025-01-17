#include "RadioComm.h"
#include "DeviceStatus.h"
#include <Arduino.h>

// Определение addressesLong
char addressesLong[30][6] = {
    "00001", "00002", "00003", "00004", "00005", "00006", "00007", "00008",
    "00009", "00010", "00011", "00012", "00013", "00014", "00015", "00016",
    "00017", "00018", "00019", "00020", "00021", "00022", "00023", "00024",
    "00025", "00026", "00027", "00028", "00029", "00030"};

RF24 radio(9, 10); // CE на D9, CSN на D10

void radio_init()
{
  radio.begin(); // активировать модуль

  radio.setAutoAck(1);      // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);  //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload(); // разрешить отсылку данных в ответ на входящий
  radio.enableDynamicPayloads();

  radio.setChannel(0x60); // выбираем канал (в котором нет шумов!)
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  radio.powerUp();       // начать работу
  radio.stopListening(); // не слушаем радиоэфир, мы передатчик
}

void ping(int deviceNum)
{
  radio.openWritingPipe(
      reinterpret_cast<const uint8_t *>(MyData[deviceNum].address));
  radio.stopListening();

  const int checkCommand = 111;
  bool sendResult = radio.write(&checkCommand, sizeof(checkCommand));

  if (sendResult)
  {
    // Устанавливаем время начала ожидания
    unsigned long startTime = millis();
    bool timeout = false;

    // Ждем ответ в течение таймаута
    while (!radio.isAckPayloadAvailable())
    {
      if (millis() - startTime > 200)
      { // Таймаут 200 мс
        timeout = true;
        break;
      }
    }

    if (!timeout && radio.isAckPayloadAvailable())
    {
      // Чтение двухзначного ответа
      byte answer[2];
      Serial.println("Answers: ");
      radio.read(&answer, sizeof(answer));
      MyData[deviceNum].connect = true;
      Serial.print("battery");
      Serial.println(answer[0]);
      Serial.print("flag");
      Serial.println(answer[1]);
      MyData[deviceNum].battery = answer[0];
      MyData[deviceNum].flag = answer[1];
    }
    else
    {
      // Ответ не получен, отмечаем устройство как не подключенное
      MyData[deviceNum].connect = false;
      MyData[deviceNum].battery = 0;
      Serial.print("SETOFF for device ");
      Serial.println(deviceNum);
    }
  }
  else
  {
    Serial.print("FALIED to send ping to device ");
    Serial.println(deviceNum);
    MyData[deviceNum].connect = false;
    MyData[deviceNum].battery = 0;
  }
  radio.startListening();
}

void Write(int myGroup, int commandSend)
{
  for (int i = 0; i < 3; i++)
  {
    int deviceIndex = i + (myGroup * 3);

    // Проверяем, подключено ли устройство
    if (!MyData[deviceIndex].connect)
    {
      Serial.print("Device not connected: ");
      Serial.println(MyData[deviceIndex].address);
      ping(deviceIndex);
      continue; // Пропускаем устройство
    }

    radio.openWritingPipe(
        reinterpret_cast<const uint8_t *>(MyData[deviceIndex].address));
    radio.stopListening();

    int commandToSend = commandSend;
    bool success = radio.write(&commandToSend, sizeof(commandToSend));

    unsigned long startTime = millis();
    bool timeout = false;

    while (!radio.isAckPayloadAvailable())
    {
      if (millis() - startTime > 200)
      { // Таймаут 200 мс
        timeout = true;
        break;
      }
    }
    //! timeout &&
    if (radio.isAckPayloadAvailable())
    {
      byte answer[2];
      radio.read(&answer, sizeof(answer));
      MyData[deviceIndex].connect = true;
      Serial.println(answer[0]);
      Serial.println(answer[1]);
      MyData[deviceIndex].battery = answer[0];
      MyData[deviceIndex].flag = answer[1];
      Serial.println("good");
    }
    else
    {
      Serial.println("Bad");
      MyData[deviceIndex].connect = false;
      ping(deviceIndex);
    }
    SetColor(deviceIndex);
  }
}
