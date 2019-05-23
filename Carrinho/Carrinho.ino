// Danilo, Francisco e Joao: Projeto do carrinho - V1 (estudando)

#include <AFMotor.h>

void calibrate();
void setMovement(short speed, short direction);

// initial speed and rotation
short initialSpeed = 100, initialDirection = 0;
short currentSpeed, currentDirection;
 
AF_DCMotor rightMotor(3);
AF_DCMotor leftMotor(4);

void setup()
{
    // calibrate();
    // delay(3000);
    Serial.begin(9600); 

    setMovement(initialSpeed, initialDirection);

}
 
void loop()
{
    return;
}

/**
 * @param speed - short from -100 to 100
 * @param direction -  short from -100 to 100
 */ 
void setMovement(short speed, short direction) // 
{
    int x = direction; 
    int y = speed;

    // TODO: search for the link
    int16_t v = (100 - abs(x)) * (y / 100) + y;
    int16_t w = (100 - abs(y)) * (x / 100) + x;

    int16_t left = (v + w) / 2;
    int16_t right = (v - w) / 2;

    // maps each value on the motor's scale
    left = map(left, -100, 100, -255, 255);
    right = map(right, -100, 100, -255, 255);
    
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