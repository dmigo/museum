
#include "Arduino.h"
#include "Lock.h"

Lock::Lock(int pin){
  _closed = false;
  _pin = pin;
  pinMode(_pin, OUTPUT);
  lock();
}
  
void Lock::lock(){
  if(!_closed){
    digitalWrite(_pin, LOW);
    _closed = true;
  }
}
  
void Lock::unlock(){
  if(_closed){
    digitalWrite(_pin, HIGH);
    _closed = false;
  }
}

