#pragma once
#include <SoftwareSerial.h>

class Bluetooth{
    SoftwareSerial BTSerial;

    public:
        Bluetooth(short rxPin, short txPin);
        String read();
        void begin(int baudRate);
};

