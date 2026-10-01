#include "imu.h"

IMUSystem imuSystem;

IMUSystem::IMUSystem()
    : initialized(false), health(DeviceHealth::UNKNOWN),
      accelX(0), accelY(0), accelZ(0),
      gyroX(0), gyroY(0), gyroZ(0),
      roll(0), pitch(0),
      gyroBiasX(0), gyroBiasY(0), gyroBiasZ(0),
      lastUpdateTime(0) {}

bool IMUSystem::init() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    if (!mpu.begin(0x68, &Wire)) {
        health = DeviceHealth::FAULT;
        initialized = false;
        return false;
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    calibrateBias();
    initialized = true;
    health = DeviceHealth::OK;
    return true;
}

void IMUSystem::calibrateBias() {
    sensors_event_t a, g, temp;
    float sumGx = 0, sumGy = 0, sumGz = 0;
    int samples = 50;

    for (int i = 0; i < samples; i++) {
        mpu.getEvent(&a, &g, &temp);
        sumGx += g.gyro.x;
        sumGy += g.gyro.y;
        sumGz += g.gyro.z;
        delay(10);
    }

    gyroBiasX = sumGx / samples;
    gyroBiasY = sumGy / samples;
    gyroBiasZ = sumGz / samples;
}

void IMUSystem::update() {
    if (!initialized) return;

    if (millis() - lastUpdateTime < 20) {
        return; // 50Hz IMU loop
    }
    float dt = (millis() - lastUpdateTime) / 1000.0f;
    lastUpdateTime = millis();

    sensors_event_t a, g, temp;
    if (!mpu.getEvent(&a, &g, &temp)) {
        health = DeviceHealth::WARNING;
        return;
    }

    accelX = a.acceleration.x;
    accelY = a.acceleration.y;
    accelZ = a.acceleration.z;

    gyroX = g.gyro.x - gyroBiasX;
    gyroY = g.gyro.y - gyroBiasY;
    gyroZ = g.gyro.z - gyroBiasZ;

    // Calculate Roll and Pitch from Accelerometer
    float accRoll = atan2(accelY, accelZ) * RAD_TO_DEG;
    float accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * RAD_TO_DEG;

    // Complementary Filter (96% Gyro integration + 4% Accelerometer angle)
    roll = 0.96f * (roll + gyroX * RAD_TO_DEG * dt) + 0.04f * accRoll;
    pitch = 0.96f * (pitch + gyroY * RAD_TO_DEG * dt) + 0.04f * accPitch;

    health = DeviceHealth::OK;
}
