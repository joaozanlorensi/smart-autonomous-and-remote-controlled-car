#pragma once
#include <AFMotor.h>

class Motor {
  AF_DCMotor rightMotor;
  AF_DCMotor leftMotor;
  short currentSpeed;
  short currentDirection;
  float leftMultiplier;
  float rightMultiplier;

public:
  Motor(short leftMotorNumber, short rightMotorNumber);
  Motor(short leftMotorNumber, short rightMotorNumber, float leftMultiplier,
        float rightMultiplier);
  void setMovement(short speed, short direction);
  void brake();
  short getCurrentSpeed() { return currentSpeed; }
  short getCurrentDirection() { return currentDirection; };
};
