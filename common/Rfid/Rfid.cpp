
#include "Arduino.h"
#include "Rfid.h"

Rfid::Rfid(int ss, int rst){
  _rfid = new MFRC522(ss, rst);
  SPI.begin();
  _rfid->PCD_Init();
}
  
int Rfid::readUid(){
  byte status;
  byte byteCount;
  byte buffer[2];
    
  byteCount = sizeof(buffer);
  int uidDec = 0;
  int uidDecTemp = 0;
  status = _rfid->PICC_RequestA(buffer, &byteCount);
    
  if (_rfid->PICC_ReadCardSerial()) {
    for (byte i = 0; i < _rfid->uid.size; i++) {  
      uidDecTemp=_rfid->uid.uidByte[i];
      uidDec=uidDec*256+uidDecTemp;
    }
  
    _rfid->PICC_ReadCardSerial();

    return uidDec;       
  }
}
