#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include <GyverButton.h>

void initButtons();
void updateButtons();
void Write(int myGroup, int commandToSend);

#define NUM_BUTTONS 10

#endif // BUTTONCONTROL_H
