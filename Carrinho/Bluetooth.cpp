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
            inChar = (char) BTSerial.read(); //read the input
            inputString += inChar;          //make a string of the characters coming on serial
        }
        while (BTSerial.available() > 0){
            BTSerial.read() ; //clear the buffer
        }    
  }
  return inputString;
}

String Bluetooth::readJoystick(short* returnValues){
    char incomingChar = "";
    char firstChar = "";
    String messageString = "";

    if(BTSerial.available()){
        delay(2);
        firstChar = (char) BTSerial.read();
        if(firstChar == START_TX){
            while(BTSerial.available()){
                    delay(1);
                    incomingChar = BTSerial.read();
                    // Greater than 127 means a communication error
                    if(incomingChar == END_TX || incomingChar > 127 )
                        break;
                    messageString += (char) incomingChar;
            }
            while (BTSerial.available() > 0){
                BTSerial.read() ; //clear the buffer
            } 
        }
        else 
            return;
    }

    if(messageString.length() == 6) {

        short x = messageString.substring(0,3).toInt() - 200;
        short y = messageString.substring(4,6).toInt() - 200;

        returnValues[0] = x;
        returnValues [1] = y;
    }

    return messageString;
}
