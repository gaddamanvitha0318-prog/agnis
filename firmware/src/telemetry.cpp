#include "telemetry.h"

TelemetrySystem telemetrySystem;

TelemetrySystem::TelemetrySystem() : lastUpdateTime(0) {}

void TelemetrySystem::update() {
    if (millis() - lastUpdateTime < 50) {
        return; // 20 Hz Telemetry update
    }
    lastUpdateTime = millis();

    currentData.batteryVoltage = powerMonitor.getVoltage();
    currentData.batteryCurrent = powerMonitor.getCurrent();
    currentData.batteryPower = powerMonitor.getPower();
    currentData.estimatedSOC = powerMonitor.getEstimatedSOC();

    currentData.frontDistance = dualUltrasonic.getFrontDistance();
    currentData.rearDistance = dualUltrasonic.getRearDistance();
    currentData.frontServoAngle = servoController.getFrontAngle();
    currentData.rearServoAngle = servoController.getRearAngle();
    currentData.frontZone = dualUltrasonic.getFrontZone();
    currentData.rearZone = dualUltrasonic.getRearZone();

    currentData.accelX = imuSystem.getAccelX();
    currentData.accelY = imuSystem.getAccelY();
    currentData.accelZ = imuSystem.getAccelZ();
    currentData.gyroX = imuSystem.getGyroX();
    currentData.gyroY = imuSystem.getGyroY();
    currentData.gyroZ = imuSystem.getGyroZ();
    currentData.roll = imuSystem.getRoll();
    currentData.pitch = imuSystem.getPitch();

    currentData.gpsFix = gpsSystem.hasFix();
    currentData.satellites = gpsSystem.getSatellites();
    currentData.latitude = gpsSystem.getLatitude();
    currentData.longitude = gpsSystem.getLongitude();
    currentData.gpsSpeed = gpsSystem.getSpeedKmh();

    currentData.mode = safetyManager.getMode();
    currentData.safetyState = safetyManager.getState();
    currentData.leftMotorSpeed = motorController.getLeftSpeed();
    currentData.rightMotorSpeed = motorController.getRightSpeed();
    currentData.estopActive = safetyManager.isEstopActive();
    currentData.uptimeMs = millis();
}
