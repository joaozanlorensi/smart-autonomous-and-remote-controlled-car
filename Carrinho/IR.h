#pragma once

class IR {
  short int pin;
  bool lastReading;

public:
  IR(short int pin);
  bool isBlack();
  bool isWhite();
  bool didReadingChange();
};