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
#include "Bluetooth.h"
#include "IR.h"

#define LEFT_MULTIPLIER 0.9
#define RIGHT_MULTIPLIER 1
#define LEFT_MOTOR_PIN 4
#define RIGHT_MOTOR_PIN 3
#define IR_PIN A2
#define BLUETOOTH_RX A0 
#define BLUETOOTH_TX A1


// initial speed and rotation
short initialSpeed = 0, initialDirection = 0;
 
Motor motor(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN, LEFT_MULTIPLIER, RIGHT_MULTIPLIER);
Bluetooth bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
IR ir(IR_PIN);

void setup()
{
    Serial.begin(9600); 
    bluetooth.begin(9600);
    motor.setMovement(initialSpeed, initialDirection);
}
 
void loop()
{
    if(ir.didReadingChange()){
        motor.setMovement(0,0);
    }
    if(!ir.isBlack()){
        remoteControl();
    } 
    return;
}

void remoteControl()
{
    String inputString = bluetooth.read();
        if(inputString == "u"){        //In case of 'u', go forward
            motor.setMovement(100, 0);
        } 
        else if(inputString == "d"){   //In case of 'd', go backwards
            motor.setMovement(-100, 0);
        }
        else if(inputString == "l"){   // In case of 'l', spin left
            motor.setMovement(0, 100);
        }
        else if(inputString == "r"){   // In case 'r', spin right
            motor.setMovement(0, -100);
        }
        else if (inputString == "0"){ // In case '0', stop
            motor.setMovement(0, 0);
        }
}
