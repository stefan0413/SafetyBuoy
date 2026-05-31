#include "Alarm.h"
#include "Button.h"

#define HOLD_TIME 3000

bool sosActive = false;
bool holdHandled = false;
unsigned long holdStartTime = 0;

void setup() {
  setupAlarm();
  setupButton();
}

void loop() {
  handleButton();

  if (sosActive && !buttonPressed()) {
    updateSos();
  } else {
    stopSignal();
  }
}

void handleButton() {
  if (buttonReleased()) {
    resetButtonHold();
    return;
  }

  if (buttonHoldJustStarted()) {
    startButtonHold();
  }

  if (holdCompleted()) {
    handleHoldAction();
  }
}

bool buttonReleased() {
  return !buttonPressed();
}

bool buttonHoldJustStarted() {
  return holdStartTime == 0;
}

void startButtonHold() {
  holdStartTime = millis();
  stopSignal();
}

void resetButtonHold() {
  holdStartTime = 0;
  holdHandled = false;
}

bool holdCompleted() {
  return !holdHandled && holdTimePassed();
}

void handleHoldAction() {
  holdHandled = true;

  if (sosActive) {
    stopSos();
  } else {
    startSos();
  }
}

void startSos(){
  fastBeeps();
  sosActive = true;
  resetSos();
}

void stopSos(){
  sosActive = false;
  resetSos();
  longBeep();
}

bool holdTimePassed(){
  return millis() - holdStartTime >= HOLD_TIME;
}
