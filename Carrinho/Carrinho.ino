/**
 *  Project: Autonomous Robot Car (ZFM-05)
 *  Class: Sensors and Actuators
 *  Eletronics Engineering - UTFPR
 * 
 *  Students:
 *     Danilo Campana Fuchs
 *     Francisco Shigueo Miamoto
 *     João Pedro Zanlorensi Cardoso 
 */

#include "Motor.h"
#include <SoftwareSerial.h>

// initial speed and rotation
short initialSpeed = 0, initialDirection = 0;
 
Motor motor(3, 4);
SoftwareSerial BTSerial(A0, A1);

String inputString;
char junk;

void setup()
{
    Serial.begin(9600); 
    BTSerial.begin(9600);
    motor.setMovement(initialSpeed, initialDirection);
}
 
void loop()
{
    readBluetooth();
    return;
}

void readBluetooth()
{
    if(BTSerial.available()){
        while(BTSerial.available()){
            char inChar = (char)BTSerial.read(); //read the input
            inputString += inChar;        //make a string of the characters coming on serial
        }
        Serial.println(inputString);
        while (BTSerial.available() > 0){
            junk = BTSerial.read() ; // Clear the buffer
        }     

        if(inputString == "a"){         //in case of 'a' turn the LED on
            digitalWrite(13, HIGH);  
            motor.setMovement(100, 0);
        } 
        else if(inputString == "b"){   //incase of 'b' turn the LED off
            digitalWrite(13, LOW);
            motor.setMovement(0, 0);
        }
  }
}