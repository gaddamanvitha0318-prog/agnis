#include "power_monitor.h"

PowerMonitor powerMonitor;

PowerMonitor::PowerMonitor()
    : batteryVoltage(12.0f), batteryCurrent(0.5f),
      batteryPower(6.0f), estimatedSOC(85.0f),
      health(DeviceHealth::OK), lastMeasureTime(0) {}

void PowerMonitor::init() {
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db); // 0 - 3.3V range
    pinMode(PIN_VOLTAGE_ADC, INPUT);
    pinMode(PIN_CURRENT_ADC, INPUT);
}

void PowerMonitor::update() {
    if (millis() - lastMeasureTime < 100) {
        return;
    }
    lastMeasureTime = millis();

    batteryVoltage = readVoltageADC();
    batteryCurrent = readCurrentADC();
    batteryPower = batteryVoltage * batteryCurrent;
    estimatedSOC = calculateSOC(batteryVoltage);

    if (batteryVoltage <= BATTERY_CRIT_VOLTAGE) {
        health = DeviceHealth::FAULT;
    } else if (batteryVoltage <= BATTERY_WARN_VOLTAGE) {
        health = DeviceHealth::WARNING;
    } else {
        health = DeviceHealth::OK;
    }
}

float PowerMonitor::readVoltageADC() {
    uint32_t rawSum = 0;
    for (int i = 0; i < 10; i++) {
        rawSum += analogRead(PIN_VOLTAGE_ADC);
        delayMicroseconds(50);
    }
    float rawAvg = rawSum / 10.0f;
    float pinVoltage = (rawAvg / 4095.0f) * 3.3f;
    return pinVoltage * VOLTAGE_DIVIDER_RATIO;
}

float PowerMonitor::readCurrentADC() {
    uint32_t rawSum = 0;
    for (int i = 0; i < 10; i++) {
        rawSum += analogRead(PIN_CURRENT_ADC);
        delayMicroseconds(50);
    }
    float rawAvg = rawSum / 10.0f;
    float pinVoltage = (rawAvg / 4095.0f) * 3.3f;
    
    // ACS712-05B sensitivity: 185mV/A, zero point ~1.65V
    float current = (pinVoltage - 1.65f) / 0.185f;
    if (current < 0.05f) current = 0.0f;
    return current;
}

float PowerMonitor::calculateSOC(float voltage) {
    // 3S Li-Ion linear approximation: 12.6V = 100%, 9.9V = 0%
    float soc = ((voltage - 9.9f) / (12.6f - 9.9f)) * 100.0f;
    return constrain(soc, 0.0f, 100.0f);
}
