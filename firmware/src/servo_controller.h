#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include "pins.h"
#include "config.h"

class ServoController {
public:
    ServoController();
    void init();
    void setFrontAngle(int angle);
    void setRearAngle(int angle);
    int getFrontAngle() const { return frontAngle; }
    int getRearAngle() const { return rearAngle; }
    void updateSweep();

private:
    int frontAngle;
    int rearAngle;
    int sweepDirection;
    unsigned long lastSweepTime;

    void writeLedcServo(uint8_t channel, int angle);
};

extern ServoController servoController;

#endif // SERVO_CONTROLLER_H
