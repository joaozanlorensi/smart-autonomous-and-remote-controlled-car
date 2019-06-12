#pragma once
#include <AFMotor.h>

class Motor{
    AF_DCMotor rightMotor;
    AF_DCMotor leftMotor;
    short currentSpeed;
    short currentDirection;
    float leftMultiplier;
    float rightMultiplier;

    public:
        Motor(short leftMotorPin, short rightMotorPin);
        Motor(short leftMotorPin, short rightMotorPin, float leftMultiplier, float rightMultiplier);
        void setMovement(short speed, short direction);
        short getCurrentSpeed(){
            return currentSpeed;
        }
        short getCurrentDirection(){
            return currentDirection;
        };
};
