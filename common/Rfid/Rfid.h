#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

class Rfid{
  private:
  MFRC522 *_rfid;
  public:
  Rfid(int ss, int rst);
  int readUid();
};

#endif
