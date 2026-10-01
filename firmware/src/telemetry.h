#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "types.h"
#include "power_monitor.h"
#include "ultrasonic.h"
#include "servo_controller.h"
#include "imu.h"
#include "gps.h"
#include "safety_manager.h"
#include "motor_controller.h"

class TelemetrySystem {
public:
    TelemetrySystem();
    void update();
    TelemetryData getData() const { return currentData; }

private:
    TelemetryData currentData;
    unsigned long lastUpdateTime;
};

extern TelemetrySystem telemetrySystem;

#endif // TELEMETRY_H
