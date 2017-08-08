#include <Wire.h>

class Slave{
  private:
  int _address;
  void static (*_handleIncomeMessage)(int);
  void static _receive(int howMany){
    while (1 < Wire.available()) { // loop through all but the last
      char c = Wire.read(); // receive byte as a character
    }
    int buffer = Wire.read();    // receive byte as an integer
    
    (*_handleIncomeMessage)(buffer);
  }
  public:
  Slave(int address, void (*handleIncomeMessage)(int)){
    _handleIncomeMessage = handleIncomeMessage;
    Wire.begin(address);
    Wire.onReceive(_receive);
  }
};

