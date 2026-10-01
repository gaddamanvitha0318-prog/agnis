#include "ultrasonic.h"

DualUltrasonicSystem dualUltrasonic;

UltrasonicSensor::UltrasonicSensor(uint8_t tPin, uint8_t ePin)
    : trigPin(tPin), echoPin(ePin), lastValidDistance(100.0f), sensorOk(true), consecutiveFailures(0) {}

void UltrasonicSensor::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

float UltrasonicSensor::readDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Pulse duration in microseconds with 25ms timeout (~400cm max range)
    unsigned long duration = pulseIn(echoPin, HIGH, 25000);

    if (duration == 0) {
        consecutiveFailures++;
        if (consecutiveFailures > 5) {
            sensorOk = false;
        }
        // Return small safe distance on failure to prevent false "clear" assumption
        return 0.0f;
    }

    float distance = (duration * 0.0343f) / 2.0f;

    // Plausibility filtering
    if (distance < 2.0f || distance > 400.0f) {
        consecutiveFailures++;
        if (consecutiveFailures > 5) sensorOk = false;
        return lastValidDistance;
    }

    consecutiveFailures = 0;
    sensorOk = true;
    lastValidDistance = distance;
    return distance;
}

DualUltrasonicSystem::DualUltrasonicSystem()
    : frontSensor(PIN_FRONT_TRIG, PIN_FRONT_ECHO),
      rearSensor(PIN_REAR_TRIG, PIN_REAR_ECHO),
      frontDistance(100.0f), rearDistance(100.0f),
      frontZone(DistanceZone::SAFE), rearZone(DistanceZone::SAFE),
      frontHealth(DeviceHealth::OK), rearHealth(DeviceHealth::OK),
      lastMeasureTime(0) {}

void DualUltrasonicSystem::init() {
    frontSensor.init();
    rearSensor.init();
}

void DualUltrasonicSystem::update() {
    if (millis() - lastMeasureTime < 60) {
        return; // 60ms interval to prevent acoustic crosstalk echoes
    }
    lastMeasureTime = millis();

    frontDistance = frontSensor.readDistance();
    frontHealth = frontSensor.isHealthy() ? DeviceHealth::OK : DeviceHealth::FAULT;
    frontZone = calculateZone(frontDistance);

    rearDistance = rearSensor.readDistance();
    rearHealth = rearSensor.isHealthy() ? DeviceHealth::OK : DeviceHealth::FAULT;
    rearZone = calculateZone(rearDistance);
}

DistanceZone DualUltrasonicSystem::calculateZone(float distanceCm) {
    if (distanceCm <= 0.0f || distanceCm < DISTANCE_EMERGENCY_CM) {
        return DistanceZone::EMERGENCY;
    } else if (distanceCm < DISTANCE_BRAKE_CM) {
        return DistanceZone::BRAKING;
    } else if (distanceCm < DISTANCE_WARNING_CM) {
        return DistanceZone::WARNING;
    } else {
        return DistanceZone::SAFE;
    }
}
