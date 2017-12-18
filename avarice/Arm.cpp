#ifndef SERVO_ARM
#define SERVO_ARM

#include <Servo.h>

class Arm {
  private:
  int _pin;
  int _start;
  int _end;
  int _position;
  Servo _servo;
  void _moveTo(int position) {
    if (position == _position)
      return;
    _position = position;
    _servo.write(position);
  }
  public:
  Arm(int pin, int start, int end) {
    _pin = pin;
    _start = start;
    _end = end;
    
    _servo.attach(_pin);
    toStart();
  }

  void toStart() {
    _moveTo(_start);
  }

  void toEnd() {
    _moveTo(_end);
  }
};

#endif
