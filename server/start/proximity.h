#define INPUT_PIN 15

#define GATE_INIT_ERROR 0
#define GATE_INIT_SUCCESS 1

#define GATE_NOT_PASSED 0
#define GATE_PASSED 1

int gate_detect() {
  if (digitalRead(INPUT_PIN) == LOW) {
      return GATE_PASSED;
  }
  else return GATE_NOT_PASSED;
}

int gate_init() {
    pinMode(INPUT_PIN, INPUT);
    if (gate_detect() == GATE_PASSED) return GATE_INIT_ERROR;
    else return GATE_INIT_SUCCESS;
}