#include "Alarm.h"
#include "Button.h"
#include "GpsModule.h"

#define LOG(msg) Serial.println(msg)
#define LOGF(label, value) \
  do {                     \
    Serial.print(label);   \
    Serial.println(value); \
  } while(0)

#define HOLD_TIME 3000
#define GPS_UPDATE_DELAY 1000

bool sosActive = false;
bool holdHandled = false;
unsigned long holdStartTime = 0;
unsigned long lastGpsPrint = 0;

HardwareSerial gpsSerial(2);
GpsModule gpsModule(&gpsSerial, 16, 17);

void setup() {
  Serial.begin(115200);

  LOG("");
  LOG("=========================");
  LOG(" DiveBeacon Booting");
  LOG("=========================");
  
  setupAlarm();
  LOG("[OK] Alarm initialized");
  
  setupButton();
  LOG("[OK] Button initialized");
  
  gpsModule.begin();
  LOG("[OK] GPS initialized");
  
  LOG("[READY] System ready");
}

void loop() {
  gpsModule.update();
  
  handleButton();

  if (sosActive && !buttonPressed()) {
    updateSos();

    handleGpsOutput();
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

  LOG("[BUTTON] Hold started");
  
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
  LOG("[SOS] Activated");
  
  fastBeeps();
  
  sosActive = true;
  resetSos();
}

void stopSos(){
  LOG("[SOS] Deactivated");
  
  sosActive = false;
  
  resetSos();
  longBeep();
}

bool holdTimePassed(){
  return millis() - holdStartTime >= HOLD_TIME;
}

void handleGpsOutput(){
  if (millis() - lastGpsPrint >= GPS_UPDATE_DELAY) {
    gpsModule.printStatus();
    lastGpsPrint = millis();
  }
}
