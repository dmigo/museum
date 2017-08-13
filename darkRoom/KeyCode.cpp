#include <Keypad.h>

class KeyCode{
private:
  static const byte _numRows = 4;
  static const byte _numCols = 3;
  char _keymap[_numRows][_numCols]= 
  {
  {'1', '2', '3'}, 
  {'4', '5', '6'}, 
  {'7', '8', '9'},
  {'*', '0', '#'}
  };
  byte _rowPins[_numRows] = {12,11,10,9};
  byte _colPins[_numCols]= {8,7,5};
  Keypad _keypad= Keypad(makeKeymap(_keymap), _rowPins, _colPins, _numRows, _numCols);
  
  static const byte _numPassword = 5; // длинна пароля 
  char _password[_numPassword] = {'3', '2', '1', '6', '7'}; // пароль 
  
  byte _currentIndex = 0;
  bool _isOpen = false;
public:
  KeyCode(){
  }
  
  void check(){
    if(_isOpen)
      return;
    
    char key = _keypad.getKey();
    if(key != NO_KEY){
      if(key == _password[_currentIndex])
        _currentIndex++;
      else
        _currentIndex = 0;
      
      _isOpen = _currentIndex == _numPassword;
    }
  }
  bool isOpen(){
    return _isOpen;
  }
};
