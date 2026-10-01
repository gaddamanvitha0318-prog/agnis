#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "pins.h"
#include "types.h"

class IMUSystem {
public:
    IMUSystem();
    bool init();
    void update();

    float getAccelX() const { return accelX; }
    float getAccelY() const { return accelY; }
    float getAccelZ() const { return accelZ; }
    float getGyroX() const { return gyroX; }
    float getGyroY() const { return gyroY; }
    float getGyroZ() const { return gyroZ; }
    float getRoll() const { return roll; }
    float getPitch() const { return pitch; }
    DeviceHealth getHealth() const { return health; }

private:
    Adafruit_MPU6050 mpu;
    bool initialized;
    DeviceHealth health;
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float roll, pitch;
    float gyroBiasX, gyroBiasY, gyroBiasZ;
    unsigned long lastUpdateTime;

    void calibrateBias();
};

extern IMUSystem imuSystem;

#endif // IMU_H
