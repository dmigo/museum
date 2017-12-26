#include <Wire.h>

#define SOLVED 1
#define NOT_SOLVED 0
#define NOT_HERE -1

const int amount = 13;
int slaves[amount] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
int leds[amount] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
int state[amount] = {NOT_HERE};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  for (int i = 0; i < amount; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

int reduceState(int oldState, int newState) {
  if(oldState == SOLVED)
    return SOLVED;
  return newState;
}

void setState(int id, bool newState) {
  state[id] = reduceState(state[id], newState);
  sendState();
  render();
}

void sendState() {
  Serial.println();
  for (int i = 0; i < amount; i++) {
    Serial.print('C');
    Serial.print(i);
    Serial.print(':');
    if(state[i] == NOT_HERE)
      Serial.print("-");
    else
      Serial.print(state[i]);
  }
}

void skipToCommand() {
  if (Serial.available() <= 0)
    return;

  while (Serial.peek() != 'C') {
    Serial.read();
  }
}

void executeCommand() {
  if (Serial.available() <= 0
      || Serial.peek() != 'C')
    return;

  int id = Serial.parseInt();
  Serial.read();
  bool newState = Serial.parseInt();

  state[id] = reduceState(state[id], newState);
}

void readState() {
  skipToCommand();
  executeCommand();
  sendState();
  render();
}

void render() {
  for (int i = 0; i < amount; i++) {
    if (state[i] == SOLVED)
      digitalWrite(leds[i], HIGH);
    else
      digitalWrite(leds[i], LOW);
  }
}

void querySlaves() {
  for (int id = 0; id < amount; id++)
  {
    Wire.requestFrom(slaves[id], 1);    // request 1 byte from slave device
    if (Wire.available()) {
      byte b = Wire.read();
      setState(id, b == SOLVED);
    }
  }
}

void loop() {
  querySlaves();
  readState();
}
