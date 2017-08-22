#include <Arduino.h>

class Sensor{
  private:
  int _pin;
  bool _solved;
  public:
  Sensor(int pin){
    _pin = pin;
    _solved = false;

    pinMode(_pin, INPUT_PULLUP);
  }
  void check(){
    _solved = digitalRead(_pin) == LOW;
  }
  bool isSolved(){
    return _solved;
  }
};

