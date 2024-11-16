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
    radio.begin();            // активировать модуль
    radio.setAutoAck(1);      // режим подтверждения приёма, 1 вкл 0 выкл
    radio.setRetries(0, 15);  //(время между попыткой достучаться, число попыток)
    radio.enableAckPayload(); // разрешить отсылку данных в ответ на входящий
                              // сигнал
    radio.setPayloadSize(6);  // размер пакета, в байтах
    // radio.openReadingPipe(1, address[0]);     //хотим слушать трубу 0
    radio.setChannel(0x60); // выбираем канал (в котором нет шумов!)
    radio.setPALevel(
        RF24_PA_MAX);              // уровень мощности передатчика. На выбор RF24_PA_MIN,
                                   // RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
    radio.setDataRate(RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS,
                                   // RF24_1MBPS, RF24_250KBPS
    radio.powerUp();               // начать работу
    radio.stopListening();         // не слушаем радиоэфир, мы передатчик
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
            int answer[2];
            radio.read(&answer, sizeof(answer));

            MyData[deviceNum].connect = true;
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
        Serial.print("Failed to send ping to device ");
        Serial.println(deviceNum);
        MyData[deviceNum].connect = false;
        MyData[deviceNum].battery = 0;
    }
    radio.startListening();
}

void Write(int myGroup, bool commandToSend)
{
    for (int i = 0; i < 3; i++)
    {
        radio.openWritingPipe(
            reinterpret_cast<const uint8_t *>(MyData[i + (myGroup * 3)].address));
        radio.stopListening();
        // и поидеии где-то здесь должен быть таймаут как в пинге чтоб отсеивать не
        // подключеные
        bool success = radio.write(
            &commandToSend,
            sizeof(commandToSend)); // можно попробывать изминить bool на int и
                                    // присылать так: -1 - опущен, 1 - поднят, 0
                                    // неизвестно, поскольу погут быть потенциальные
                                    // проблемы с индекацией в SetColor()
        bool returnFlag;
        if (success && radio.isAckPayloadAvailable())
        {
            radio.read(&returnFlag, sizeof(returnFlag));
            MyData[i].flag = returnFlag;
        }
        else
        {
            ping(i);
        }
    }
}
