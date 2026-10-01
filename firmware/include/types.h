#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

// Operating Modes
enum class OperatingMode {
    MANUAL,
    ASSIST,
    AUTO
};

// System Safety States
enum class SafetyState {
    BOOT,
    SELF_TEST,
    READY,
    MANUAL,
    ASSIST,
    AUTO,
    WARNING,
    BRAKING,
    EMERGENCY_STOP,
    COMMUNICATION_LOSS,
    LOW_BATTERY,
    CRITICAL_BATTERY,
    FAULT
};

// Motor Directions
enum class DriveDirection {
    STOP,
    FORWARD,
    REVERSE,
    TURN_LEFT,
    TURN_RIGHT,
    PIVOT_LEFT,
    PIVOT_RIGHT
};

// Obstacle Zone Levels
enum class DistanceZone {
    SAFE,       // > 80 cm
    WARNING,    // 40 - 80 cm
    BRAKING,    // 20 - 40 cm
    EMERGENCY   // < 20 cm
};

// Subsystem Health Status
enum class DeviceHealth {
    UNKNOWN,
    OK,
    WARNING,
    FAULT
};

// Structure for telemetry data
struct TelemetryData {
    // Battery & Power
    float batteryVoltage;
    float batteryCurrent;
    float batteryPower;
    float estimatedSOC;
    
    // Distances
    float frontDistance;
    float rearDistance;
    int frontServoAngle;
    int rearServoAngle;
    DistanceZone frontZone;
    DistanceZone rearZone;
    
    // IMU Data
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float roll, pitch;
    
    // GPS Data
    bool gpsFix;
    int satellites;
    double latitude;
    double longitude;
    float gpsSpeed;
    
    // Vehicle State
    OperatingMode mode;
    SafetyState safetyState;
    int leftMotorSpeed;
    int rightMotorSpeed;
    
    // System Health
    DeviceHealth imuHealth;
    DeviceHealth gpsHealth;
    DeviceHealth frontUltrasonicHealth;
    DeviceHealth rearUltrasonicHealth;
    DeviceHealth batteryHealth;
    bool estopActive;
    bool cameraOnline;
    
    unsigned long uptimeMs;
};

// System Event Log Record
struct EventLog {
    unsigned long timestamp;
    String type;
    String message;
};

#endif // TYPES_H
