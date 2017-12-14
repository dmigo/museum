class Waiter {
  private:
    bool _isWaiting = false;
    long _since = 0;
    long _limit;
  public:
    Waiter(long limit) {
      _limit = limit;
    }

    void start() {
      _since = millis();  
      _isWaiting = true;
    }
    void drop() {
      _isWaiting = false;
    }

    bool isFinished() {
      return _isWaiting &&
        _since + _limit < millis();
    }
};

#include <Wire.h>

#define DELAY 3000 // задержка перед включением
//#define ADDRESS 

#define SOLVED 1
#define NOT_SOLVED 0

const int sensorsCount = 3;
const int sensorPins[3] = {4,5,6};

Waiter* waiter = new Waiter(DELAY);

void setup() {
  for (int i = 0; i < sensorsCount; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
    attachInterrupt(0, stateChanged, CHANGE);
  }
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop() {

}

void requestEvent() {
  if (waiter->isFinished())
    Wire.write(SOLVED);
  else
    Wire.write(NOT_SOLVED);
}

bool isOpen() {
  bool state = true;
  for (int i = 0; i < sensorsCount; i++) {
    state = state && digitalRead(sensorPins[i]) == LOW;
  }
  return state;
}

void stateChanged() {
  if(isOpen()) {
    waiter->start();
  }
  else {
    waiter->drop();
  }
}

