#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "pins.h"
#include "config.h"
#include "types.h"

class MotorController {
public:
    MotorController();
    void init();
    void setLeftMotor(int targetSpeed);
    void setRightMotor(int targetSpeed);
    void drive(int forwardSpeed, int turnValue);
    void stopMotors();
    void brakeMotors();
    void updateRamp();

    int getLeftSpeed() const { return currentLeftSpeed; }
    int getRightSpeed() const { return currentRightSpeed; }
    bool isEmergencyStopped() const { return emergencyStopped; }
    void setEmergencyStop(bool stopState);

private:
    int targetLeftSpeed;
    int targetRightSpeed;
    int currentLeftSpeed;
    int currentRightSpeed;
    bool emergencyStopped;
    unsigned long lastRampTime;

    void applyRawLeft(int speed);
    void applyRawRight(int speed);
};

extern MotorController motorController;

#endif // MOTOR_CONTROLLER_H
