#include "rdm630.h"
#include <Wire.h>
#include <Keypad.h>

#define SOLVED 1
#define NOT_SOLVED 0
#define ADDRESS 9

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

class RfidLock{
private:
  rdm630 *_rfid = new rdm630(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
  unsigned long _key;
  bool _isOpen = false;
  
  bool _isRfidAvailable(){
    return _rfid->available();
  }
  int _getRfid(){
    byte data[6];
    byte length;
    
    _rfid->getData(data, length);
    unsigned long result = 
      ((unsigned long int)data[1]<<24) + 
      ((unsigned long int)data[2]<<16) + 
      ((unsigned long int)data[3]<<8) + 
      data[4];

    return result;
  }
  
public:
  RfidLock(unsigned long key){
    _rfid->begin();
    _key = key;
  }
  
  void check(){
    if(_isOpen)
      return;
      
    if(_isRfidAvailable()){
      Serial.println("Data available");
      long result = _getRfid();
      Serial.print("Rfid: ");
      Serial.println(result);
      _isOpen = result == _key;
    }
  }
  
  bool isOpen(){
    return _isOpen;
  }
};

RfidLock *rfidLock;
KeyCode* keyCode;


void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  rfidLock = new RfidLock(32157); //сюда кладем правильный ключ
  keyCode = new KeyCode();
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.println("Starting...");
}

void loop()
{ 
  rfidLock->check();
  keyCode->check();
}

void requestEvent() {
  Serial.println("data requested");
  if(keyCode->isOpen() && rfidLock->isOpen())
    Wire.write(SOLVED); 
  else
    Wire.write(NOT_SOLVED);
}

