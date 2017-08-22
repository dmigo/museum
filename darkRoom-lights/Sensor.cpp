#include <Arduino.h>

class Sensor{
  private:
  int _pin;
  public:
  Sensor(int pin){
    _pin = pin;

    pinMode(_pin, INPUT_PULLUP);
  }
  bool isSolved(){
    return digitalRead(_pin) == LOW;
  }
};

