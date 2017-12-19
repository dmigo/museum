#ifndef GAME_STATE_INDICATION
#define GAME_STATE_INDICATION
#define BLINK_DELAY 500

#include "SimpleIndicator.cpp"
#include "BlinkingIndicator.cpp"

class GameStateIndication{
  private:
  SimpleIndicator* _green;
  SimpleIndicator* _red;
  BlinkingIndicator* _redBlink;
  
  public:
  GameStateIndication(int greenPin, int redPin){
    _green = new SimpleIndicator(greenPin);
    _red = new SimpleIndicator(redPin);
    
    _redBlink = new BlinkingIndicator(_red, BLINK_DELAY, BLINK_DELAY);
  }

  void check(){
    _redBlink->check();
  }
  
  void win(){
    _green->switchOn();
  }
  void fail(){
    _redBlink->blinkNTimes(3);
  }
};

#endif

