#include "Bluetooth.h"

Bluetooth::Bluetooth(short rxPin, short txPin): BTSerial(rxPin, txPin)
{

}

void Bluetooth::begin(int baudRate)
{
    BTSerial.begin(baudRate);
}

String Bluetooth::read()
{
    String inputString = "";
    char inChar = "";
    if(BTSerial.available()){
        while(BTSerial.available()){
            inChar = (char)BTSerial.read(); //read the input
            inputString += inChar;          //make a string of the characters coming on serial
        }
        while (BTSerial.available() > 0){
            BTSerial.read() ; //clear the buffer
        }    
  }
  return inputString;
}

