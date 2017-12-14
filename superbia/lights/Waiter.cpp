#ifndef KEYCODE
#define KEYCODE

#include <Arduino.h>

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

#endif
