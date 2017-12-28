#ifndef CONDITIONAL_TIMER
#define CONDITIONAL_TIMER

#include <Arduino.h>

class ConditionalTimer {
private:
  bool (*_condition)();
  void (*_onReached)();
  bool _conditionFulfilled = false;
  unsigned long _switched = 0;
  unsigned long _interval = 0;

  void _setState(bool conditionFulfilled){
    if(conditionFulfilled != _conditionFulfilled){
      _switched = millis();
      _conditionFulfilled = conditionFulfilled;
    }
  }

  bool _isReached(){
    return _conditionFulfilled && millis() > _switched + _interval;
  }

  void _reached(){
    if(_onReached!=0L)
      _onReached();
  }
public:
  ConditionalTimer(bool (*condition)(), unsigned long interval){
    _onReached = 0L;
    _condition = condition;
    _interval = interval;
  }

  void onReached(void (*onReached)()){
    _onReached = onReached;
  }

  void check(){
    bool conditionFulfilled = _condition();

    _setState(conditionFulfilled);

    if(_isReached())
      _reached();
  }
};

#endif
