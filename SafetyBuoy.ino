#define LED 26
#define BUZZER 25
#define BUTTON 4

#define HOLD_TIME 3000

struct Step {
  int duration;
  int pauseAfter;
};

Step sosPattern[] = {
  {200, 200}, {200, 200}, {200, 400},
  {600, 200}, {600, 200}, {600, 400},
  {200, 200}, {200, 200}, {200, 2000}
};

const int SOS_STEPS = 9;

bool sosActive = false;
bool holdHandled = false;
bool signalOn = false;

unsigned long holdStartTime = 0;
unsigned long lastChange = 0;

int currentStep = 0;

void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  handleButton();

  if (sosActive && !buttonPressed()) {
    updateSos();
  } else {
    stopSignal();
  }
}

bool buttonPressed() {
  return digitalRead(BUTTON) == LOW;
}

void handleButton() {
  if (!buttonPressed()) {
    holdStartTime = 0;
    holdHandled = false;
    return;
  }

  if (holdStartTime == 0) {
    holdStartTime = millis();
    stopSignal();
  }

  if (!holdHandled && millis() - holdStartTime >= HOLD_TIME) {
    holdHandled = true;

    if (!sosActive) {
      fastBeeps();
      sosActive = true;
      resetSos();
    } else {
      sosActive = false;
      resetSos();
      longBeep();
    }
  }
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

void fastBeeps() {
  for (int i = 0; i < 3; i++) {
    blockingBeep(120);
    delay(100);
  }
}

void longBeep() {
  blockingBeep(1000);
}

void blockingBeep(int duration) {
  digitalWrite(BUZZER, HIGH);
  digitalWrite(LED, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);
}
