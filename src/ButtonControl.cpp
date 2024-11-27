#include "ButtonControl.h"
#include "RadioComm.h"

const int buttonPins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, A4, A5, A6, A7};

typedef struct {
  GButton button;
  bool state;
  bool oldState;

} ButtonStruct;

ButtonStruct buttons[NUM_BUTTONS];

void initButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].button = GButton(buttonPins[i]);
    buttons[i].state = false;
    buttons[i].oldState = false;
    buttons[i] = {GButton(buttonPins[i])};
  }
}

void updateButtons() {
  // Обработка состояния кнопок
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].button.tick();
    buttons[i].state = buttons[i].button.isHold();

    if (buttons[i].state != buttons[i].oldState) {
      if (buttons[i].button.isHold()) {
        Serial.print("SetUp: ");
        Serial.println(i);
        Write(i, 257);
      } else {
        Serial.print("SetDown: ");
        Serial.println(i);
        Write(i, 99);
      }
      buttons[i].oldState = buttons[i].state;
    }
  }
}
