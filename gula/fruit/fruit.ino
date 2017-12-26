#define ADDRESS // 22-23
#define SOLVED 1
#define NOT_SOLVED 0

#define UID 10775353  //айдишник нужной нфцшки

#include <Wire.h>
#include "Puzzle.cpp"
#include "Rfid.cpp"

Rfid* rfid;
Puzzle* puzzle = new Puzzle();

void setup()
{
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  rfid = new Rfid();
  rfid->onActive(rfidActive);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  rfid->check();
}

void requestEvent() {
  if (puzzle->isSolved())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

void rfidActive(unsigned long uid) {
  if (uid == UID)
    puzzle->solve();
}
