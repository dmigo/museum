#include <Wire.h>

#define SOLVED 1
#define NOT_SOLVED 0

const int amount = 3;
int slaves[amount] = { 8, 9, 10};
int leds[amount] = { 8, 9, 10}; 

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  for(int i=0; i<amount; i++){
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  for(int i = 0; i < amount; i++)
  {
    Wire.requestFrom(slaves[i], 1);    // request 1 byte from slave device
    if (Wire.available()) { 
      byte b = Wire.read(); 
      Serial.print(b); 
      Serial.print(" from "); 
      Serial.println(slaves[i]);
      if(b == SOLVED)
        digitalWrite(leds[i], HIGH);
      else
        digitalWrite(leds[i], LOW);
    }
  }
}
