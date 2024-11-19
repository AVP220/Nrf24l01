#include "ButtonControl.h"

const int buttonPins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, A1, A2, A3, A4};

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
      } else {
        Serial.print("SetDown: ");
        Serial.println(i);
      }
      buttons[i].oldState = buttons[i].state;
    }
  }
}
