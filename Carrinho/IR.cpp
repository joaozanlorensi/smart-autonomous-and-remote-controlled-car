#include "IR.h"
#include "Arduino.h"

IR::IR(short pin){
    this->pin = pin;
    pinMode(pin, INPUT);
    lastReading = (bool) digitalRead(pin);
}

bool IR::isBlack(){
    // Active in zero;
    lastReading = (bool) digitalRead(pin);
    return !lastReading;
}

bool IR::isWhite(){
    lastReading = (bool) digitalRead(pin);
    return lastReading;
}

bool IR::didReadingChange(){
    bool currentReading = (bool) digitalRead(pin);
    bool hasReadingChanged = currentReading != lastReading;
    lastReading = currentReading;
    return hasReadingChanged;
}