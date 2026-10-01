#include "autonomous.h"

AutonomousNavigator autoNav;

AutonomousNavigator::AutonomousNavigator()
    : subState(AutoSubState::FORWARD),
      leftClearance(0), rightClearance(0),
      stateTimer(0), turnDirection(0) {}

void AutonomousNavigator::init() {
    reset();
}

void AutonomousNavigator::reset() {
    subState = AutoSubState::FORWARD;
    stateTimer = millis();
    servoController.setFrontAngle(SERVO_CENTER_ANGLE);
}

void AutonomousNavigator::update() {
    if (safetyManager.getMode() != OperatingMode::AUTO) {
        return;
    }

    if (safetyManager.getState() == SafetyState::EMERGENCY_STOP ||
        safetyManager.getState() == SafetyState::CRITICAL_BATTERY) {
        motorController.stopMotors();
        return;
    }

    switch (subState) {
        case AutoSubState::FORWARD:
            handleForward();
            break;
        case AutoSubState::STOP_AND_SCAN:
            handleStopAndScan();
            break;
        case AutoSubState::SCAN_LEFT:
            handleScanLeft();
            break;
        case AutoSubState::SCAN_RIGHT:
            handleScanRight();
            break;
        case AutoSubState::DECIDE:
            handleDecide();
            break;
        case AutoSubState::TURNING:
            handleTurning();
            break;
        default:
            reset();
            break;
    }
}

void AutonomousNavigator::handleForward() {
    float dist = dualUltrasonic.getFrontDistance();

    if (dist > DISTANCE_WARNING_CM) {
        servoController.setFrontAngle(SERVO_CENTER_ANGLE);
        motorController.drive(160, 0); // Steady autonomous cruise speed
    } else {
        motorController.stopMotors();
        subState = AutoSubState::STOP_AND_SCAN;
        stateTimer = millis();
        eventLogger.log("AUTO_SCAN", "Obstacle ahead, initiating sweep scan");
    }
}

void AutonomousNavigator::handleStopAndScan() {
    motorController.stopMotors();
    if (millis() - stateTimer > 400) {
        servoController.setFrontAngle(SERVO_MIN_ANGLE); // Point Left (30 deg)
        stateTimer = millis();
        subState = AutoSubState::SCAN_LEFT;
    }
}

void AutonomousNavigator::handleScanLeft() {
    if (millis() - stateTimer > 400) {
        leftClearance = dualUltrasonic.getFrontDistance();
        servoController.setFrontAngle(SERVO_MAX_ANGLE); // Point Right (150 deg)
        stateTimer = millis();
        subState = AutoSubState::SCAN_RIGHT;
    }
}

void AutonomousNavigator::handleScanRight() {
    if (millis() - stateTimer > 400) {
        rightClearance = dualUltrasonic.getFrontDistance();
        servoController.setFrontAngle(SERVO_CENTER_ANGLE);
        stateTimer = millis();
        subState = AutoSubState::DECIDE;
    }
}

void AutonomousNavigator::handleDecide() {
    eventLogger.log("AUTO_DECIDE", "Left: " + String(leftClearance) + " cm, Right: " + String(rightClearance) + " cm");

    if (leftClearance < DISTANCE_BRAKE_CM && rightClearance < DISTANCE_BRAKE_CM) {
        // Both blocked -> Pivot 180 degrees
        turnDirection = -1;
        stateTimer = millis() + 600; // Extra turn duration
    } else if (leftClearance >= rightClearance) {
        turnDirection = -1; // Turn left
    } else {
        turnDirection = 1;  // Turn right
    }

    subState = AutoSubState::TURNING;
    stateTimer = millis();
}

void AutonomousNavigator::handleTurning() {
    if (turnDirection < 0) {
        motorController.drive(0, -180); // Pivot left
    } else {
        motorController.drive(0, 180);  // Pivot right
    }

    if (millis() - stateTimer > 800) { // 800ms pivot duration
        motorController.stopMotors();
        subState = AutoSubState::FORWARD;
        stateTimer = millis();
    }
}
