#include <Arduino.h>
#include "Button.cpp"
#include "Indicators.cpp"


#define DEBOUNCE_TIME 100

class Tactile {
  private:
    Button* _button;
    SimpleIndicator* _indicator;
  public:
    Tactile(int buttonPin, int indicatorPin) {
      _button = new Button(buttonPin);
      _indicator = new SimpleIndicator(indicatorPin);
      _indicator->switchOff();
    }

    void check(){
      _button->check();
    }

    void onPress(void (*callback)(int)) {
      _indicator->switchOn();
      _button->onPress(callback);
    }
    void onRelease(void (*callback)(int)) {
      _indicator->switchOff();
      _button->onRelease(callback);
    }

    void deactivate(long milliseconds) {
      _button->deactivate(milliseconds);
    }
    void deactivate() {
      _button->deactivate();
    }
    void activate() {
      _button->activate();
    }
};


