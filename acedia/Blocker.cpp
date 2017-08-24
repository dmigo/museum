#include <Arduino.h>

class Blocker {
  private:
    bool _blocked;
    long _whenBlocked;
    long _duration;

  public:
    Blocker() {
      _blocked = false;
    }
    void block(long milliseconds) {
      _whenBlocked = millis();
      _duration = milliseconds;
      _blocked = true;
    }
    void check() {
      if (!_blocked)
        return;
      if (millis() > _whenBlocked + _duration)
        _blocked = false;
    }
    void block() {
      _blocked = true;
    }
    void unblock() {
      _blocked = false;
    }
    bool isBlocked() {
      return _blocked;
    }
};

