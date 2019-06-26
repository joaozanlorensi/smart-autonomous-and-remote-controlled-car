#include "RFID.h"

RFID::RFID(short ssPin, short rstPin) : mfrc522(ssPin, rstPin) {}

void RFID::begin() {
  SPI.begin();
  mfrc522.PCD_Init();
  delay(5);
}

String RFID::readUIDFromTag() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return "";

  if (!mfrc522.PICC_ReadCardSerial())
    return "";

  String uidString = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidString.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidString.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  return uidString;
}

void RFID::dumpDataToSerial() {
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}