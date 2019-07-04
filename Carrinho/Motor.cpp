#include "Motor.h"

#define abs(x) ((x) > 0 ? (x) : -(x))

float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Motor::Motor(short leftMotorNumber, short rightMotorNumber)
    : leftMotor(leftMotorNumber), rightMotor(rightMotorNumber) {}

Motor::Motor(short leftMotorNumber, short rightMotorNumber,
             float leftMultiplier, float rightMultiplier)
    : leftMotor(leftMotorNumber), rightMotor(rightMotorNumber),
      leftMultiplier(leftMultiplier), rightMultiplier(rightMultiplier) {}

/**
 * @param speed - short from -100 to 100
 * @param direction -  short from -100 to 100
 * If passed speed 0 and direction 0, will RELEASE motor, not brake it.
 */
void Motor::setMovement(short speed, short direction) //
{
  float x = direction;
  float y = speed;

  // TODO: search for the link
  float v = (100 - abs(x)) * (y / 100) + y; // 140
  float w = (100 - abs(y)) * (x / 100) + x; // 0

  float leftPercentage = (v + w) / 2;  // 70
  float rightPercentage = (v - w) / 2; // 70

  // maps each value on the motor's scale
  int16_t left =
      (int16_t)(leftMultiplier * map(leftPercentage, -100, 100, -255, 255));
  int16_t right =
      (int16_t)(rightMultiplier * map(rightPercentage, -100, 100, -255, 255));

  // sets the motors' direction
  if (right > 0) {
    rightMotor.run(FORWARD);
  } else {
    rightMotor.run(BACKWARD);
  }
  if (left > 0) {
    leftMotor.run(FORWARD);
  } else {
    leftMotor.run(BACKWARD);
  }

  currentSpeed = speed;
  currentDirection = direction;

  rightMotor.setSpeed(abs(right));
  leftMotor.setSpeed(abs(left));
}

/**
 * Will brake the motor making sure it stays in position
 */
void Motor::brake() {
  rightMotor.run(BRAKE);
  leftMotor.run(BRAKE);
  rightMotor.setSpeed(0);
  leftMotor.setSpeed(0);

  currentSpeed = 0;
  currentDirection = 0;
}