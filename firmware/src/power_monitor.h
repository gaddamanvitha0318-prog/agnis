#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Arduino.h>
#include "pins.h"
#include "config.h"
#include "types.h"

class PowerMonitor {
public:
    PowerMonitor();
    void init();
    void update();

    float getVoltage() const { return batteryVoltage; }
    float getCurrent() const { return batteryCurrent; }
    float getPower() const { return batteryPower; }
    float getEstimatedSOC() const { return estimatedSOC; }
    DeviceHealth getHealth() const { return health; }

private:
    float batteryVoltage;
    float batteryCurrent;
    float batteryPower;
    float estimatedSOC;
    DeviceHealth health;
    unsigned long lastMeasureTime;

    float readVoltageADC();
    float readCurrentADC();
    float calculateSOC(float voltage);
};

extern PowerMonitor powerMonitor;

#endif // POWER_MONITOR_H
