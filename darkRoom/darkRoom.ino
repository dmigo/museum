#define ADDRESS 9
#define DROP_BUTTON A3
#define DEBOUNCE_TIME 100

#include <Wire.h>
#include "RfidLock.cpp"
#include "KeyCode.cpp"
#include "Indicators.cpp"
#include "Sensor.cpp"

#define SOLVED 1
#define NOT_SOLVED 0

RfidLock* rfidLock;
KeyCode* rightCode;

SimpleIndicator* green1;
SimpleIndicator* green2;
SimpleIndicator* red;
BlinkingIndicator* redBlink;
Sensor* dropButton;

static const byte sizeRightPassword = 4; // длинна правильного пароля
char rightPassword[sizeRightPassword] = {'2', '0', '1', '7'}; // правильный пароль

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");

  dropButton = new Sensor(DROP_BUTTON, DEBOUNCE_TIME); // кнопка сброса
  dropButton->onDrop(dropCode);

  rfidLock = new RfidLock(27446); //сюда кладем правильный rfid-ключ

  rightCode = new KeyCode(sizeRightPassword, rightPassword);
  rightCode->onSuccess(codeSolved);
  rightCode->onFailure(codeFailed);

  green1 = new SimpleIndicator(A0);
  green2 = new SimpleIndicator(A1);
  red = new SimpleIndicator(A2);

  redBlink = new BlinkingIndicator(red, 500, 500);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Started.");
}

void loop()
{
  dropButton->check();
  rfidLock->check();
  redBlink->check();
  rightCode->check();

  if (rfidLock->isOpen())
    green1->switchOn();
  else
    green1->switchOff();
}

void requestEvent() {
  if (rightCode->isSolved() && rfidLock->isOpen())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

void codeSolved() {
  green2->switchOn();
}
void codeFailed() {
  redBlink->blinkNTimes(3);
}

void dropCode(int pin) {
  Serial.println("Drop");
  rightCode->restart();
}

