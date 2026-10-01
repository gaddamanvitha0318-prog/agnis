#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <Arduino.h>
#include "types.h"
#include "ultrasonic.h"
#include "servo_controller.h"
#include "imu.h"
#include "gps.h"
#include "power_monitor.h"
#include "motor_controller.h"
#include "safety_manager.h"

class SystemDiagnostics {
public:
    SystemDiagnostics();
    bool runSelfTest();
    String runDiagnosticReport();
    bool testMotorLeft(bool confirm);
    bool testMotorRight(bool confirm);

private:
    bool selfTestPassed;
};

extern SystemDiagnostics diagnostics;

#endif // DIAGNOSTICS_H
