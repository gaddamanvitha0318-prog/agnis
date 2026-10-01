#include "safety_manager.h"

SafetyManager safetyManager;

SafetyManager::SafetyManager()
    : currentMode(OperatingMode::MANUAL),
      currentState(SafetyState::BOOT),
      estopActive(false),
      lastCommandTime(0),
      lastBuzzerToggleTime(0),
      buzzerState(false) {}

void SafetyManager::init() {
    pinMode(PIN_ESTOP_SENSE, INPUT_PULLUP);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_BLUE, LOW);
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_BUZZER, LOW);

    currentState = SafetyState::SELF_TEST;
    lastCommandTime = millis();
}

bool SafetyManager::checkPhysicalEstop() {
    // Physical Normally-Closed switch:
    // Closed (normal) -> pin pulled LOW or HIGH depending on wiring.
    // When pressed / open circuit -> GPIO 19 reads HIGH via pull-up.
    return (digitalRead(PIN_ESTOP_SENSE) == HIGH);
}

void SafetyManager::triggerSoftwareEstop() {
    estopActive = true;
    motorController.setEmergencyStop(true);
    currentState = SafetyState::EMERGENCY_STOP;
    eventLogger.log("ESTOP", "Software E-stop triggered!");
}

void SafetyManager::resetEstop() {
    if (!checkPhysicalEstop()) {
        estopActive = false;
        motorController.setEmergencyStop(false);
        currentState = SafetyState::READY;
        eventLogger.log("ESTOP", "E-stop reset successfully");
    } else {
        eventLogger.log("ESTOP_WARN", "Cannot reset: Physical E-stop button is still engaged!");
    }
}

void SafetyManager::updateCommandHeartbeat() {
    lastCommandTime = millis();
}

void SafetyManager::setMode(OperatingMode newMode) {
    if (currentState == SafetyState::EMERGENCY_STOP || currentState == SafetyState::CRITICAL_BATTERY) {
        eventLogger.log("MODE_REJECT", "Cannot change mode during emergency/battery cutoff!");
        return;
    }
    currentMode = newMode;
    eventLogger.log("MODE_CHANGE", "Operating mode changed");
}

void SafetyManager::update() {
    // 1. PRIORITY 1: Physical E-Stop
    if (checkPhysicalEstop()) {
        if (!estopActive) {
            estopActive = true;
            motorController.setEmergencyStop(true);
            currentState = SafetyState::EMERGENCY_STOP;
            eventLogger.log("ESTOP", "Physical Mushroom E-stop engaged!");
        }
    }

    if (estopActive) {
        currentState = SafetyState::EMERGENCY_STOP;
        motorController.stopMotors();
        updateStatusIndicators();
        updateBuzzer(100); // Rapid beep
        return;
    }

    // 2. PRIORITY 2: Critical Battery Cutoff
    if (powerMonitor.getVoltage() <= BATTERY_CRIT_VOLTAGE) {
        currentState = SafetyState::CRITICAL_BATTERY;
        motorController.stopMotors();
        updateStatusIndicators();
        updateBuzzer(250);
        return;
    }

    // 3. PRIORITY 3: Communication Timeout Check (In MANUAL or ASSIST mode)
    if (currentMode != OperatingMode::AUTO && (millis() - lastCommandTime > COMMS_TIMEOUT_MS)) {
        currentState = SafetyState::COMMUNICATION_LOSS;
        motorController.stopMotors();
        updateStatusIndicators();
        updateBuzzer(500);
        return;
    }

    // 4. PRIORITY 4: Obstacle Safety Overrides (In ASSIST Mode)
    DistanceZone fZone = dualUltrasonic.getFrontZone();
    DistanceZone rZone = dualUltrasonic.getRearZone();

    if (fZone == DistanceZone::EMERGENCY || rZone == DistanceZone::EMERGENCY) {
        currentState = SafetyState::EMERGENCY_STOP;
        motorController.stopMotors();
        updateStatusIndicators();
        updateBuzzer(150);
        return;
    } else if (fZone == DistanceZone::BRAKING || rZone == DistanceZone::BRAKING) {
        currentState = SafetyState::BRAKING;
        if (currentMode == OperatingMode::ASSIST) {
            motorController.stopMotors(); // Force brake in Assist mode
        }
        updateStatusIndicators();
        updateBuzzer(300);
        return;
    } else if (fZone == DistanceZone::WARNING || rZone == DistanceZone::WARNING) {
        currentState = SafetyState::WARNING;
        updateStatusIndicators();
        updateBuzzer(800);
        return;
    }

    // 5. Normal Mode States
    if (currentMode == OperatingMode::AUTO) {
        currentState = SafetyState::AUTO;
    } else if (currentMode == OperatingMode::ASSIST) {
        currentState = SafetyState::ASSIST;
    } else {
        currentState = SafetyState::MANUAL;
    }

    updateStatusIndicators();
    digitalWrite(PIN_BUZZER, LOW);
}

void SafetyManager::updateStatusIndicators() {
    switch (currentState) {
        case SafetyState::READY:
        case SafetyState::MANUAL:
            digitalWrite(PIN_LED_GREEN, HIGH);
            digitalWrite(PIN_LED_BLUE, LOW);
            digitalWrite(PIN_LED_RED, LOW);
            break;
        case SafetyState::ASSIST:
        case SafetyState::AUTO:
            digitalWrite(PIN_LED_GREEN, LOW);
            digitalWrite(PIN_LED_BLUE, HIGH);
            digitalWrite(PIN_LED_RED, LOW);
            break;
        case SafetyState::WARNING:
            digitalWrite(PIN_LED_GREEN, HIGH);
            digitalWrite(PIN_LED_BLUE, LOW);
            digitalWrite(PIN_LED_RED, HIGH); // Amber combination
            break;
        case SafetyState::BRAKING:
        case SafetyState::EMERGENCY_STOP:
        case SafetyState::COMMUNICATION_LOSS:
        case SafetyState::CRITICAL_BATTERY:
        case SafetyState::FAULT:
        default:
            digitalWrite(PIN_LED_GREEN, LOW);
            digitalWrite(PIN_LED_BLUE, LOW);
            digitalWrite(PIN_LED_RED, HIGH);
            break;
    }
}

void SafetyManager::updateBuzzer(int intervalMs) {
    if (millis() - lastBuzzerToggleTime >= (unsigned long)intervalMs) {
        lastBuzzerToggleTime = millis();
        buzzerState = !buzzerState;
        digitalWrite(PIN_BUZZER, buzzerState ? HIGH : LOW);
    }
}
