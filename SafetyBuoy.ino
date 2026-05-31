#define BUZZER 25
#define MORSE_DOT 200
#define MORSE_DASH 600
#define MORSE_PAUSE 400
#define MORSE_TINY_PAUSE 200
#define SOS_DELAY 2000


void setup() {
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  sosSignal();
}

void sosSignal(){
  // S (...)
  beep(MORSE_DOT);
  beep(MORSE_DOT);
  beep(MORSE_DOT);

  delay(MORSE_PAUSE);

  // O (---)
  beep(MORSE_DASH);
  beep(MORSE_DASH);
  beep(MORSE_DASH);

  delay(MORSE_PAUSE);

  // S (...)
  beep(MORSE_DOT);
  beep(MORSE_DOT);
  beep(MORSE_DOT);

  delay(SOS_DELAY);
}

void beep(int duration) {
  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
  delay(MORSE_TINY_PAUSE);
}
