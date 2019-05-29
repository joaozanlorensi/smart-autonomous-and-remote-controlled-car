#pragma once
#include <AFMotor.h>

class Motor{
    AF_DCMotor rightMotor;
    AF_DCMotor leftMotor;
    short currentSpeed;
    short currentDirection;

    public:
        Motor(short leftMotorPin, short rightMotorPin);
        void setMovement(short speed, short direction);
        short getCurrentSpeed(){
            return currentSpeed;
        }
        short getCurrentDirection(){
            return currentDirection;
        };
};
