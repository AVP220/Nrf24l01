#include "ButtonControl.h"

const int buttonPins[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, A1, A2, A3, A4};

typedef struct {
  GButton button;
  bool state;
} ButtonStruct;

ButtonStruct buttons[NUM_BUTTONS];

void initButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i] = {GButton(buttonPins[i])};
  }
}

void updateButtons() {
  // Обработка состояния кнопок
}
