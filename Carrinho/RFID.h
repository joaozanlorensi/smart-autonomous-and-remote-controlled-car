#pragma once
#include <MFRC522.h>
#include <SPI.h>

class RFID {
  MFRC522 mfrc522;

public:
  RFID(short ssPin, short rstPin);
  void begin();
  void dumpDataToSerial();
  String readUIDFromTag();
};