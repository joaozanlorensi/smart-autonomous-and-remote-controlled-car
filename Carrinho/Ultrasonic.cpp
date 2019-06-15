#include "Ultrasonic.h"
#include "Arduino.h"

Ultrasonic::Ultrasonic(short trigPin, short echoPin): trigPin(trigPin), echoPin(echoPin){
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float Ultrasonic::getDistance(){
    // Clear sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);

    // Triggers the sensor
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    float travelDuration = pulseIn(echoPin, HIGH);
    return travelDuration * 0.0343 / 2;
}

float Ultrasonic::getAveragedDistance(short numOfSamples){
    float accumulator = 0;
    for(short i = 0; i < numOfSamples; i++){
        accumulator += getDistance();
    }

    return accumulator / numOfSamples;
}
