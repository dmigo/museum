#ifndef DURABLE_INDICATOR
#define DURABLE_INDICATOR

#include <Arduino.h>
#include "SimpleIndicator.cpp"


class DurableIndicator{
  private:
  SimpleIndicator *_indicator;
  long _start;
  long _duration;
  
  bool _isOn(){
    return millis() < _start + _duration;
  }
  
  public:
  DurableIndicator(SimpleIndicator* indicator){
    _indicator = indicator;

    _start = 0;
    _duration = 0;
  }
  
  void check(){
    bool isOn = _isOn();

    if(isOn)
      _indicator->switchOn();
    else
      _indicator->switchOff();
  }
  
  void lightUp(long duration){
    _duration = duration;
    _start = millis();
  }
};

#endif


