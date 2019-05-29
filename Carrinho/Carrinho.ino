// Danilo, Francisco e Joao: Projeto do carrinho - V1 (estudando)

#include "Motor.h"

// initial speed and rotation
short initialSpeed = 100, initialDirection = 0;
 
Motor motor(3, 4);

void setup()
{
    Serial.begin(9600); 
    motor.setMovement(initialSpeed, initialDirection);
}
 
void loop()
{
    return;
}