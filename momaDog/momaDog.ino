#include <Wire.h>

#define SOLVED 1
#define NOT_SOLVED 0

const int amount = 4;
int slaves[amount] = { 8, 9, 10, 11};
int leds[amount] = { 8, 9, 10, 11};
bool state[amount] = {false};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial1.begin(115200);
  Serial2.begin(9600);
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
  for (int i = 0; i < amount; i++) {
    Serial1.print('C');
    Serial1.print(i);
    Serial1.print(':');
    Serial1.print(state[i]);
  }
}

void skipToCommand() {
  if (Serial1.available() <= 0)
    return;
    
  while(Serial1.peek() != 'C'){
    Serial1.read();
  }
}

void executeCommand() {
  if (Serial1.available() <= 0
  || Serial1.peek() != 'C')
    return;

  int id = Serial1.parseInt();
  Serial1.read();
  bool newState = Serial1.parseInt();

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
      Serial2.print(b); 
      Serial2.print(" from "); 
      Serial2.println(slaves[id]);

      setState(id, b == SOLVED);
    }
  }
}

void loop() {
  querySlaves();
  readState();
}
