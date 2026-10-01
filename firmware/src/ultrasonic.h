#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "pins.h"
#include "config.h"
#include "types.h"

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);
    void init();
    float readDistance();
    bool isHealthy() const { return sensorOk; }

private:
    uint8_t trigPin;
    uint8_t echoPin;
    float lastValidDistance;
    bool sensorOk;
    int consecutiveFailures;
};

class DualUltrasonicSystem {
public:
    DualUltrasonicSystem();
    void init();
    void update();

    float getFrontDistance() const { return frontDistance; }
    float getRearDistance() const { return rearDistance; }
    DistanceZone getFrontZone() const { return frontZone; }
    DistanceZone getRearZone() const { return rearZone; }
    DeviceHealth getFrontHealth() const { return frontHealth; }
    DeviceHealth getRearHealth() const { return rearHealth; }

private:
    UltrasonicSensor frontSensor;
    UltrasonicSensor rearSensor;
    float frontDistance;
    float rearDistance;
    DistanceZone frontZone;
    DistanceZone rearZone;
    DeviceHealth frontHealth;
    DeviceHealth rearHealth;
    unsigned long lastMeasureTime;

    DistanceZone calculateZone(float distanceCm);
};

extern DualUltrasonicSystem dualUltrasonic;

#endif // ULTRASONIC_H
