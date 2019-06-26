#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

#define START_TX 0x02
#define END_TX 0x03

class Bluetooth {
  SoftwareSerial BTSerial;

public:
  Bluetooth(short rxPin, short txPin);
  String read();
  String readJoystick(short *returnValues);
  void begin(int baudRate);
};
