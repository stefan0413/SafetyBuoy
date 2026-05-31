#include "Arduino.h"
#include "Pins.h"
#include "Alarm.h"

#define SOS_STEPS 9

struct Step {
  int duration;
  int pauseAfter;
};

Step sosPattern[] = {
  {200, 200}, {200, 200}, {200, 400},
  {600, 200}, {600, 200}, {600, 400},
  {200, 200}, {200, 200}, {200, 2000}
};

bool signalOn = false;
unsigned long lastChange = 0;
int currentStep = SOS_STEPS - 1;

void setupAlarm() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  stopSignal();
}

void updateSos() {
  unsigned long now = millis();

  if (signalOn) {
    if (now - lastChange >= sosPattern[currentStep].duration) {
      stopSignal();
      signalOn = false;
      lastChange = now;
    }
  } else {
    if (now - lastChange >= sosPattern[currentStep].pauseAfter) {
      currentStep = (currentStep + 1) % SOS_STEPS;
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED, HIGH);
      signalOn = true;
      lastChange = now;
    }
  }
}

void resetSos() {
  currentStep = SOS_STEPS - 1;
  signalOn = false;
  lastChange = millis();
  stopSignal();
}

void stopSignal() {
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);
}

void blockingBeep(int duration) {
  digitalWrite(BUZZER, HIGH);
  digitalWrite(LED, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);
}

void fastBeeps() {
  for (int i = 0; i < 3; i++) {
    blockingBeep(120);
    delay(100);
  }
}

void longBeep() {
  blockingBeep(1000);
}
