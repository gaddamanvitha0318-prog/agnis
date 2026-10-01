#include "motor_controller.h"

MotorController motorController;

MotorController::MotorController() 
    : targetLeftSpeed(0), targetRightSpeed(0),
      currentLeftSpeed(0), currentRightSpeed(0),
      emergencyStopped(false), lastRampTime(0) {}

void MotorController::init() {
    pinMode(PIN_MOTOR_IN1, OUTPUT);
    pinMode(PIN_MOTOR_IN2, OUTPUT);
    pinMode(PIN_MOTOR_IN3, OUTPUT);
    pinMode(PIN_MOTOR_IN4, OUTPUT);

    // Setup LEDC hardware PWM channels for ESP32 DevKit V1
    ledcSetup(0, LEDC_PWM_FREQ_MOTOR, LEDC_PWM_RES_MOTOR);
    ledcAttachPin(PIN_MOTOR_ENA, 0);

    ledcSetup(1, LEDC_PWM_FREQ_MOTOR, LEDC_PWM_RES_MOTOR);
    ledcAttachPin(PIN_MOTOR_ENB, 1);

    stopMotors();
}

void MotorController::setLeftMotor(int targetSpeed) {
    if (emergencyStopped) {
        targetLeftSpeed = 0;
        return;
    }
    targetLeftSpeed = constrain(targetSpeed, -255, 255);
}

void MotorController::setRightMotor(int targetSpeed) {
    if (emergencyStopped) {
        targetRightSpeed = 0;
        return;
    }
    targetRightSpeed = constrain(targetSpeed, -255, 255);
}

void MotorController::drive(int forwardSpeed, int turnValue) {
    if (emergencyStopped) {
        stopMotors();
        return;
    }

    forwardSpeed = constrain(forwardSpeed, -255, 255);
    turnValue = constrain(turnValue, -255, 255);

    // Differential steering calculation
    int left = forwardSpeed + turnValue;
    int right = forwardSpeed - turnValue;

    setLeftMotor(left);
    setRightMotor(right);
}

void MotorController::stopMotors() {
    targetLeftSpeed = 0;
    targetRightSpeed = 0;
    currentLeftSpeed = 0;
    currentRightSpeed = 0;
    applyRawLeft(0);
    applyRawRight(0);
}

void MotorController::brakeMotors() {
    targetLeftSpeed = 0;
    targetRightSpeed = 0;
    currentLeftSpeed = 0;
    currentRightSpeed = 0;
    
    // H-Bridge active electronic braking (IN1=HIGH, IN2=HIGH; IN3=HIGH, IN4=HIGH)
    digitalWrite(PIN_MOTOR_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IN2, HIGH);
    ledcWrite(0, 255);

    digitalWrite(PIN_MOTOR_IN3, HIGH);
    digitalWrite(PIN_MOTOR_IN4, HIGH);
    ledcWrite(1, 255);
}

void MotorController::setEmergencyStop(bool stopState) {
    emergencyStopped = stopState;
    if (emergencyStopped) {
        stopMotors();
    }
}

void MotorController::updateRamp() {
    if (millis() - lastRampTime < MOTOR_RAMP_INTERVAL_MS) {
        return;
    }
    lastRampTime = millis();

    if (emergencyStopped) {
        stopMotors();
        return;
    }

    // Left Motor Ramping
    if (currentLeftSpeed < targetLeftSpeed) {
        currentLeftSpeed = min(currentLeftSpeed + MOTOR_RAMP_STEP, targetLeftSpeed);
    } else if (currentLeftSpeed > targetLeftSpeed) {
        currentLeftSpeed = max(currentLeftSpeed - MOTOR_RAMP_STEP, targetLeftSpeed);
    }

    // Right Motor Ramping
    if (currentRightSpeed < targetRightSpeed) {
        currentRightSpeed = min(currentRightSpeed + MOTOR_RAMP_STEP, targetRightSpeed);
    } else if (currentRightSpeed > targetRightSpeed) {
        currentRightSpeed = max(currentRightSpeed - MOTOR_RAMP_STEP, targetRightSpeed);
    }

    applyRawLeft(currentLeftSpeed);
    applyRawRight(currentRightSpeed);
}

void MotorController::applyRawLeft(int speed) {
    if (speed > 0) {
        digitalWrite(PIN_MOTOR_IN1, HIGH);
        digitalWrite(PIN_MOTOR_IN2, LOW);
        ledcWrite(0, speed);
    } else if (speed < 0) {
        digitalWrite(PIN_MOTOR_IN1, LOW);
        digitalWrite(PIN_MOTOR_IN2, HIGH);
        ledcWrite(0, -speed);
    } else {
        digitalWrite(PIN_MOTOR_IN1, LOW);
        digitalWrite(PIN_MOTOR_IN2, LOW);
        ledcWrite(0, 0);
    }
}

void MotorController::applyRawRight(int speed) {
    if (speed > 0) {
        digitalWrite(PIN_MOTOR_IN3, HIGH);
        digitalWrite(PIN_MOTOR_IN4, LOW);
        ledcWrite(1, speed);
    } else if (speed < 0) {
        digitalWrite(PIN_MOTOR_IN3, LOW);
        digitalWrite(PIN_MOTOR_IN4, HIGH);
        ledcWrite(1, -speed);
    } else {
        digitalWrite(PIN_MOTOR_IN3, LOW);
        digitalWrite(PIN_MOTOR_IN4, LOW);
        ledcWrite(1, 0);
    }
}
