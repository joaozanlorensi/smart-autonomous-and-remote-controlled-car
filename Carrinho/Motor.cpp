#include "Motor.h"

#define abs(x) ((x)>0?(x):-(x))

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Motor::Motor(short leftMotorNumber, short rightMotorNumber):
    leftMotor(leftMotorNumber), rightMotor(rightMotorNumber){
}

Motor::Motor(short leftMotorNumber, short rightMotorNumber, float leftMultiplier, float rightMultiplier): 
    leftMotor(leftMotorNumber), rightMotor(rightMotorNumber), 
    leftMultiplier(leftMultiplier), rightMultiplier(rightMultiplier){
}

/**
 * @param speed - short from -100 to 100
 * @param direction -  short from -100 to 100
 */ 
void Motor::setMovement(short speed, short direction) // 
{
    int x = direction; 
    int y = speed;

    // TODO: search for the link
    int16_t v = (100 - abs(x)) * (y / 100) + y;
    int16_t w = (100 - abs(y)) * (x / 100) + x;

    int16_t left = (v + w) / 2;
    int16_t right = (v - w) / 2;

    // maps each value on the motor's scale
    left = leftMultiplier * map(left, -100, 100, -255, 255);
    right = rightMultiplier * map(right, -100, 100, -255, 255);
    
    // sets the motors' direction
    if(right > 0)
    {
        rightMotor.run(FORWARD);
    }
    else
    {
        rightMotor.run(BACKWARD);
    }
    if(left > 0)
    {
        leftMotor.run(FORWARD);
    }
    else
    {
        leftMotor.run(BACKWARD);
    }

    currentSpeed = speed;
    currentDirection = direction;

    rightMotor.setSpeed(abs(right));
    leftMotor.setSpeed(abs(left));
}
