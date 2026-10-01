#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include <Arduino.h>
#include "types.h"
#include "motor_controller.h"
#include "ultrasonic.h"
#include "servo_controller.h"
#include "safety_manager.h"
#include "event_logger.h"

enum class AutoSubState {
    FORWARD,
    CHECK_FRONT,
    STOP_AND_SCAN,
    SCAN_LEFT,
    SCAN_RIGHT,
    DECIDE,
    TURNING,
    RESCAN
};

class AutonomousNavigator {
public:
    AutonomousNavigator();
    void init();
    void update();
    void reset();

private:
    AutoSubState subState;
    float leftClearance;
    float rightClearance;
    unsigned long stateTimer;
    int turnDirection; // -1 for left, +1 for right

    void handleForward();
    void handleStopAndScan();
    void handleScanLeft();
    void handleScanRight();
    void handleDecide();
    void handleTurning();
};

extern AutonomousNavigator autoNav;

#endif // AUTONOMOUS_H
