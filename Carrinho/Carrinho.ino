/**
 *  Project: Autonomous Robot Car (ZFM-05)
 *  Class: Sensors and Actuators
 *  Eletronics Engineering - UTFPR
 *
 *  Students:
 *     Danilo Campana Fuchs
 *     Francisco Shigueo Miamoto
 *     João Pedro Zanlorensi Cardoso
 */

#include "Bluetooth.h"
#include "IR.h"
#include "Motor.h"
#include "RFID.h"
#include "Ultrasonic.h"
#include "Buzzer.h"

#define LEFT_MOTOR_NUMBER 4
#define RIGHT_MOTOR_NUMBER 3
#define LEFT_MULTIPLIER 0.9
#define RIGHT_MULTIPLIER 1

#define IR_PIN A2

#define BLUETOOTH_RX A0
#define BLUETOOTH_TX A1

#define TRIG_PIN A3
#define ECHO_PIN A4

#define RFID_RST_PIN 9
#define RFID_SS_PIN 10

#define BUZZER_PIN 22 // Joao: *redefine pin

#define MODE_MANUAL 0
#define MODE_AUTONOMOUS 1

#define STEP_NULL 0
#define STEP_SEARCH_TARGET 1
#define STEP_GOTO_TARGET 2
#define STEP_READ_RFID 3
#define STEP_RETRY_RFID_BACKWARDS 4
#define STEP_RETRY_RFID_SWIVEL 5
#define STEP_LEAVE_ZONE 6

#define TURNING_RIGHT -1
#define TURNING_LEFT 1

#define MANUAL_SPEED 100
#define MANUAL_TURNING_SPEED 80

#define AUTO_SPEED 60
#define AUTO_BACKWARDS_SPEED -80
#define AUTO_TURNING_SPEED 70

#define FAR_DISTANCE_THRESHOLD 50
#define CLOSE_DISTANCE_THRESHOLD 5
#define RFID_READ_TIMEOUT 500
#define RFID_BACKWARDS_DURATION 800
#define RFID_SWIVEL_DURATION 400

#define BUZZER_SONG 0

short mode = MODE_MANUAL;

typedef struct {
  short step;
  unsigned long timeRef;
  short searchDirection;
} AutonomousState;

AutonomousState autonomousState;

short initialSpeed = 0, initialDirection = 0;

Motor motor(LEFT_MOTOR_NUMBER, RIGHT_MOTOR_NUMBER, LEFT_MULTIPLIER,
            RIGHT_MULTIPLIER);
Bluetooth bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
IR ir(IR_PIN);
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);
RFID rfid(RFID_SS_PIN, RFID_RST_PIN);
Buzzer buzzer(BUZZER_PIN, BUZZER_SONG);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  rfid.begin();
  motor.setMovement(initialSpeed, initialDirection);

  autonomousState.step = STEP_NULL;
  autonomousState.timeRef = millis();
  autonomousState.searchDirection = TURNING_LEFT;
}

void loop() {
  if (ir.didReadingChange()) {
    motor.setMovement(0, 0);
    if (ir.isBlack()) {
      buzzer.sing(THEME_CHALLENGE); 
      mode = MODE_AUTONOMOUS;
      autonomousState.step = STEP_SEARCH_TARGET;
    } else {
      mode = MODE_MANUAL;
      autonomousState.step = STEP_NULL;
    }
  }
  if (mode == MODE_MANUAL) {
    autonomousState.searchDirection =
        remoteControl(autonomousState.searchDirection);
  } else if (mode == MODE_AUTONOMOUS) {
    autonomousState = autonomousControl(autonomousState);
  }
}

short remoteControl(short currentDirection) {
  String inputString = bluetooth.read();

  if (inputString == "u") {
    // Go forward
    motor.setMovement(MANUAL_SPEED, 0);
  } else if (inputString == "d") {
    // Go backwards
    motor.setMovement(-MANUAL_SPEED, 0);
  } else if (inputString == "l") {
    // Spin left
    motor.setMovement(0, MANUAL_TURNING_SPEED);
  } else if (inputString == "r") {
    // Spin right
    motor.setMovement(0, -MANUAL_TURNING_SPEED);
  } else if (inputString == "0") {
    // Stop
    motor.setMovement(0, 0);
  } else if (inputString == "k") {
    return TURNING_RIGHT;
  } else if (inputString == "j") {
    return TURNING_LEFT;
  }

  return currentDirection;
}

/**
 * Receives the current step, runs the necessary action
 * and returns the next desired step
 */
AutonomousState autonomousControl(AutonomousState state) {
  short currentStep = state.step;
  unsigned long timeRef = state.timeRef;
  short searchDirection = state.searchDirection;

  short nextStep = STEP_NULL;
  unsigned long nextTimeRef = timeRef;
  short nextSearchDirection = searchDirection;

  if (currentStep == STEP_NULL) {
    // Woops
    nextStep = STEP_NULL;
  }
  if (currentStep == STEP_SEARCH_TARGET) {
    float distance = ultrasonic.getDistance();
    if (distance > FAR_DISTANCE_THRESHOLD) {
      // Nothing on sight
      motor.setMovement(0, searchDirection * AUTO_TURNING_SPEED);
      nextStep = STEP_SEARCH_TARGET;
    } else {
      // Look! Straight ahead!
      motor.setMovement(0, 0);
      nextStep = STEP_GOTO_TARGET;
    }
  }
  if (currentStep == STEP_GOTO_TARGET) {
    float distance = ultrasonic.getDistance();
    if (distance < CLOSE_DISTANCE_THRESHOLD) {
      // Found target! Yaaay!!
      motor.setMovement(0, 0);
      nextStep = STEP_READ_RFID;
      nextTimeRef = millis();
    } else if (distance > FAR_DISTANCE_THRESHOLD) {
      // Lost target :(
      motor.setMovement(0, 0);
      nextStep = STEP_SEARCH_TARGET;
    } else {
      // On my way!
      motor.setMovement(AUTO_SPEED, 0);
      nextStep = STEP_GOTO_TARGET;
    }
  }
  if (currentStep == STEP_READ_RFID) {
    String tagUID = rfid.readUIDFromTag();
    bool successfulReading = tagUID != "";
    if (successfulReading) {
      buzzer.sing(THEME_VICTORY); 
      nextStep = STEP_LEAVE_ZONE;
    } else {
      if ((millis() - timeRef) > RFID_READ_TIMEOUT) {
        nextStep = STEP_RETRY_RFID_BACKWARDS;
        nextTimeRef = millis();
      } else {
        nextStep = STEP_READ_RFID;
      }
    }
  }
  if (currentStep == STEP_RETRY_RFID_BACKWARDS) {
    if ((millis() - timeRef) < RFID_BACKWARDS_DURATION) {
      motor.setMovement(AUTO_BACKWARDS_SPEED, 0);
      nextStep = STEP_RETRY_RFID_BACKWARDS;
    } else {
      nextStep = STEP_RETRY_RFID_SWIVEL;
      nextTimeRef = millis();
    }
  }
  if (currentStep == STEP_RETRY_RFID_SWIVEL) {
    if ((millis() - timeRef) < RFID_SWIVEL_DURATION) {
      motor.setMovement(0, -searchDirection * AUTO_TURNING_SPEED);
      nextStep = STEP_RETRY_RFID_SWIVEL;
    } else {
      nextStep = STEP_SEARCH_TARGET;
    }
  }
  if (currentStep == STEP_LEAVE_ZONE) {
    // Just keep rolling back
    motor.setMovement(AUTO_BACKWARDS_SPEED, 0);
    nextStep = STEP_LEAVE_ZONE;
  }

  return (AutonomousState){.step = nextStep,
                           .timeRef = nextTimeRef,
                           .searchDirection = nextSearchDirection};
}