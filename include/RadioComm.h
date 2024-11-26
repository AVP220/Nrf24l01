#ifndef RADIOCOMM_H
#define RADIOCOMM_H

#include <RF24.h>

// Объявление addressesLong как extern
extern char addressesLong[30][6];

void radio_init();
void ping(int deviceNum);
void Write(int myGroup, bool commandToSend);
void SetColor();
void TrueColor();

extern RF24 radio;

#endif // RADIOCOMM_H
