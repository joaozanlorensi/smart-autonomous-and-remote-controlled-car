#pragma once

class Ultrasonic {
    short trigPin, echoPin;

public: 
    Ultrasonic(short trigPin, short echoPin);
    float getDistance();
    float getAveragedDistance(short numOfSamples);
};