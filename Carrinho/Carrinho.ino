// Danilo, Francisco e Joao: Projeto do carrinho - V1 (estudando)
/*
#define FORWARD HIGH

void calibrate();
void setMotorDirection();
void setMotorSpeed();



// offsets
int leftOffset = 0, rightOffset = 0, center = 0;
// speed pins
int speed1 = 10, speed2 = 11, direction1 = 12, direction2 = 13;
// initial speed and rotation
int startSpeed = 70, rotate = 30;
// threshold
int threshold = 5;
int left = startSpeed, right = startSpeed;

int motorsOn = true;
int button = 2;

// All motors are initially off, until the button is pressed.
// Var motorsOff must be kept true in case we don't use buttons


void setup()
{
    pinMode(button, INPUT_PULLUP);
    // defining the motor pins as outputs
    pinMode(9, OUTPUT);
    pinMode(speed1, OUTPUT);
    pinMode(speed2, OUTPUT);
    pinMode(direction1, OUTPUT);
    pinMode(direction2, OUTPUT);

    // calibrating the sensors
    calibrate();
    delay(3000);

    // the direction of the motors are defined as forward
    
    // defining both motor's speeds

}
*/

//Programa : Teste de motor DC12V com motor shield ponte H
//Autor : Equipe FILIPEFLOP
 
#include <AFMotor.h>

#define SPEED_MIN -100
#define SPEED_MAX 100
#define DIRECTION_MIN -100
#define DIRECTION_MAX 100
 
AF_DCMotor rightMotor(1); //Seleciona o motor 1
AF_DCMotor leftMotor(2);
 
void setup()
{
    Serial.begin(9600);
    setMovement(0, 0);
    delay(10000);
    setMovement(100, 0);
    delay(10000);
    setMovement(0, 100);

}
 
void loop()
{}

void setMovement(uint16_t speed, uint16_t direction)
{
    
    Serial.print("Entrada\nspd: ");
    Serial.print(speed);
    Serial.print(" dir: ");
    Serial.print(direction);
    Serial.println();

    int x = direction;
    int y = speed;

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

    rightMotor.setSpeed(abs(right));
    leftMotor.setSpeed(abs(left));
    

    Serial.print("Resultado\n direita: ");
    Serial.print(right);
    Serial.print(" esquerda: ");
    Serial.print(left);
    Serial.println();


}