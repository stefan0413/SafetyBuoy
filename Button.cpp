#include "Arduino.h"
#include "Pins.h"
#include "Button.h"

void setupButton() {
  pinMode(BUTTON, INPUT_PULLUP);
}

bool buttonPressed() {
  return digitalRead(BUTTON) == LOW;
}
