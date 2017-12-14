#include <Wire.h>

#define SOLVED 1
#define NOT_SOLVED 0

const int amount = 6;
int slaves[amount] = { 8, 9, 10, 11, 12, 13};
int leds[amount] = { 8, 9, 10, 11, 12, 13};
bool state[amount] = {false};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  for(int i=0; i<amount; i++){
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void setState(int id, bool newState) {
  state[id] = state[id] || newState;
  sendState();
  render();
}

void sendState() {
  Serial.println();
  for (int i = 0; i < amount; i++) {
    Serial.print('C');
    Serial.print(i);
    Serial.print(':');
    Serial.print(state[i]);
  }
}

void skipToCommand() {
  if (Serial.available() <= 0)
    return;
    
  while(Serial.peek() != 'C'){
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

  state[id] = state[id] || newState;
}

void readState() {
  skipToCommand();
  executeCommand();
  sendState();
  render();
}

void render() {
  for (int i = 0; i < amount; i++) {
    if(state[i])
        digitalWrite(leds[i], HIGH);
      else
        digitalWrite(leds[i], LOW);
  }
}

void querySlaves(){
  for(int id = 0; id < amount; id++)
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
