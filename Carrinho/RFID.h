#pragma once
#include <SPI.h>
#include <MFRC522.h>


class RFID {
    MFRC522 mfrc522;

    public:
        RFID(short ssPin, short rstPin);
        void begin();
        void dumpDataToSerial();
        String getUIDFromTag();
};