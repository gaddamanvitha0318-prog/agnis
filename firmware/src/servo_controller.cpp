#include "servo_controller.h"

ServoController servoController;

// LEDC Setup parameters for 50Hz Servo control (20ms period)
// 16-bit resolution -> 65535 max duty cycle value.
// 0.5ms pulse (0 deg)  -> (0.5 / 20.0) * 65535 = 1638 duty
// 1.5ms pulse (90 deg) -> (1.5 / 20.0) * 65535 = 4915 duty
// 2.5ms pulse (180 deg)-> (2.5 / 20.0) * 65535 = 8192 duty

ServoController::ServoController()
    : frontAngle(SERVO_CENTER_ANGLE), rearAngle(SERVO_CENTER_ANGLE),
      sweepDirection(1), lastSweepTime(0) {}

void ServoController::init() {
    // Setup LEDC Channel 2 for Front Servo
    ledcSetup(2, LEDC_PWM_FREQ_SERVO, LEDC_PWM_RES_SERVO);
    ledcAttachPin(PIN_SERVO_FRONT, 2);

    // Setup LEDC Channel 3 for Rear Servo
    ledcSetup(3, LEDC_PWM_FREQ_SERVO, LEDC_PWM_RES_SERVO);
    ledcAttachPin(PIN_SERVO_REAR, 3);

    setFrontAngle(SERVO_CENTER_ANGLE);
    setRearAngle(SERVO_CENTER_ANGLE);
}

void ServoController::setFrontAngle(int angle) {
    frontAngle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    writeLedcServo(2, frontAngle);
}

void ServoController::setRearAngle(int angle) {
    rearAngle = constrain(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
    writeLedcServo(3, rearAngle);
}

void ServoController::updateSweep() {
    if (millis() - lastSweepTime < SERVO_SWEEP_INTERVAL_MS) {
        return;
    }
    lastSweepTime = millis();

    int nextAngle = frontAngle + (sweepDirection * SERVO_SWEEP_STEP);
    if (nextAngle >= SERVO_MAX_ANGLE) {
        nextAngle = SERVO_MAX_ANGLE;
        sweepDirection = -1;
    } else if (nextAngle <= SERVO_MIN_ANGLE) {
        nextAngle = SERVO_MIN_ANGLE;
        sweepDirection = 1;
    }

    setFrontAngle(nextAngle);
    setRearAngle(180 - nextAngle); // Rear sweeps in opposite phase
}

void ServoController::writeLedcServo(uint8_t channel, int angle) {
    // Map angle (0 to 180 deg) to 16-bit duty cycle (1638 to 8192)
    uint32_t duty = 1638 + ((uint32_t)angle * (8192 - 1638) / 180);
    ledcWrite(channel, duty);
}
