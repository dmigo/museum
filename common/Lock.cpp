
#include "Lock.h"

class Lock{
  
public:
  Lock(int pin){
    _closed = false;
    _pin = pin;
    pinMode(_pin, OUTPUT);
    lock();
  }
  
  void lock(){
    if(!_closed){
      digitalWrite(_pin, LOW);//todo look it up
      _closed = true;
    }
  }
  
  void unlock(){
    if(_closed){
      digitalWrite(_pin, HIGH);//todo look it up
      _closed = false;
    }
  }
};i

