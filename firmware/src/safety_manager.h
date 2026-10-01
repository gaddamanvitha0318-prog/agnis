#ifndef SAFETY_MANAGER_H
#define SAFETY_MANAGER_H

#include <Arduino.h>
#include "pins.h"
#include "config.h"
#include "types.h"
#include "motor_controller.h"
#include "ultrasonic.h"
#include "power_monitor.h"
#include "event_logger.h"

class SafetyManager {
public:
    SafetyManager();
    void init();
    void update();
    void updateCommandHeartbeat();
    void setMode(OperatingMode newMode);
    void triggerSoftwareEstop();
    void resetEstop();

    OperatingMode getMode() const { return currentMode; }
    SafetyState getState() const { return currentState; }
    bool isEstopActive() const { return estopActive; }

private:
    OperatingMode currentMode;
    SafetyState currentState;
    bool estopActive;
    unsigned long lastCommandTime;
    unsigned long lastBuzzerToggleTime;
    bool buzzerState;

    void updateStatusIndicators();
    void updateBuzzer(int intervalMs);
    bool checkPhysicalEstop();
};

extern SafetyManager safetyManager;

#endif // SAFETY_MANAGER_H
