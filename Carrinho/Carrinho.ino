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
#include "Ultrasonic.h"
#include "RFID.h"

#define LEFT_MOTOR_NUMBER 4
#define RIGHT_MOTOR_NUMBER 3
#define LEFT_MULTIPLIER 0.9
#define RIGHT_MULTIPLIER 1

#define IR_PIN A2

#define BLUETOOTH_RX A0 
#define BLUETOOTH_TX A1

#define TRIG_PIN A3
#define ECHO_PIN A4

#define RFID_RST_PIN 9
#define RFID_SS_PIN 10

#define MODE_MANUAL         0
#define MODE_AUTONOMOUS     1

#define STEP_NULL           0
#define STEP_SEARCH_TARGET  1
#define STEP_GOTO_TARGET    2
#define STEP_READ_RFID      3
#define STEP_LEAVE_ZONE     4

short mode = MODE_MANUAL;
short step = STEP_NULL;

short initialSpeed = 0, initialDirection = 0;
 
Motor motor(LEFT_MOTOR_NUMBER, RIGHT_MOTOR_NUMBER, LEFT_MULTIPLIER, RIGHT_MULTIPLIER);
Bluetooth bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
IR ir(IR_PIN);
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);

void setup()
{
    Serial.begin(9600); 
    bluetooth.begin(9600);
    rfid.begin();
    motor.setMovement(initialSpeed, initialDirection);
}
 
void loop()
{
    if (ir.didReadingChange()) {
        motor.setMovement(0,0);
        if (ir.isBlack()) {
            step = STEP_SEARCH_TARGET;
        } else {
            step = STEP_NULL;
        }
    }
    if (ir.isWhite()) {
        mode = MODE_MANUAL;
        remoteControl();
    } else {
        mode = MODE_AUTONOMOUS;
        step = autonomousControl(step);
    }
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

/**
 * Receives the current step, runs the necessary action
 * and returns the next desired step
 */
short autonomousControl(short curretStep) {
    if (curretStep == STEP_NULL) {
        // Woops
        return STEP_NULL;
    }
    if (curretStep == STEP_SEARCH_TARGET) {
        float distance = ultrasonic.getDistance();
        if (distance > 100) {
            // Nothing on sight
            motor.setMovement(0, 100);
            return STEP_SEARCH_TARGET;
        } else {
            // Look! Straight ahead!
            motor.setMovement(0, 0);
            return STEP_GOTO_TARGET;
        }
    }
    if (curretStep == STEP_GOTO_TARGET) {
        float distance = ultrasonic.getDistance();
        if (distance < 10) {
            // Found target! Yaaay!!
            motor.setMovement(0, 0);
            return STEP_READ_RFID;
        }
        else if (distance > 100) {
            // Lost target :(
            motor.setMovement(0, 0);
            return STEP_SEARCH_TARGET;
        } else {
            // On my way!
            motor.setMovement(100, 0);
            return STEP_GOTO_TARGET;
        }
    }
    if (curretStep == STEP_READ_RFID) {
        String tagUID = rfid.readUIDFromTag();
        if(tagUID != "")
            return STEP_LEAVE_ZONE;
        else
            return STEP_READ_RFID;        
    }
    if (curretStep == STEP_LEAVE_ZONE) {
        // Just keep rolling back
        motor.setMovement(-100, 0);
        return STEP_LEAVE_ZONE;
    }

    return STEP_NULL;
}