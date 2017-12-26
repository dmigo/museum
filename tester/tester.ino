#define ADDRESS // не забудь сюда адркс

#include <Wire.h>

#define SOLVED 1
#define NOT_SOLVED 0

#define STATE_PIN 4 // пин для смены состояния (по умолчанию шлем SOLVED)

void setup()
{
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  pinMode(STATE_PIN, INPUT_PULLUP);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);

  Serial.println("Started.");
}

void loop()
{
}

void requestEvent() {
  if(digitalRead(STATE_PIN) == LOW)
    Wire.write(NOT_SOLVED);
  else
    Wire.write(SOLVED); 
}

