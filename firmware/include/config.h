#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// AGNIS-4WD CONFIGURATION PARAMETERS
// ============================================================================

// Wi-Fi Access Point Configuration
#define WIFI_AP_SSID            "AGNIS-4WD"
#define WIFI_AP_PASS            "AGNIS2026"
#define WIFI_AP_CHANNEL         6
#define WIFI_MAX_CLIENTS        4

// Communication Watchdog Timeout (ms)
#define COMMS_TIMEOUT_MS        750

// Motor Ramp Acceleration Limit (PWM step change per loop iteration)
#define MOTOR_RAMP_STEP         15
#define MOTOR_RAMP_INTERVAL_MS   20

// Safety Distance Thresholds (cm)
#define DISTANCE_SAFE_CM        80.0f
#define DISTANCE_WARNING_CM     40.0f
#define DISTANCE_BRAKE_CM       20.0f
#define DISTANCE_EMERGENCY_CM   15.0f

// Battery Voltage Telemetry Thresholds (for 3S Li-Ion: 11.1V Nominal, 12.6V Max, 9.9V Cutoff)
#define BATTERY_WARN_VOLTAGE    10.5f
#define BATTERY_CRIT_VOLTAGE     9.9f
#define VOLTAGE_DIVIDER_RATIO    4.0f   // R1=30k, R2=10k -> (30+10)/10 = 4.0

// Servo Calibration Angles (Degrees)
#define SERVO_MIN_ANGLE          30
#define SERVO_CENTER_ANGLE       90
#define SERVO_MAX_ANGLE         150
#define SERVO_SWEEP_STEP         15
#define SERVO_SWEEP_INTERVAL_MS 250

// PWM Channels & Frequencies
#define LEDC_PWM_FREQ_MOTOR    5000     // 5 kHz for motor H-bridge PWM
#define LEDC_PWM_RES_MOTOR     8        // 8-bit resolution (0-255)
#define LEDC_PWM_FREQ_SERVO    50       // 50 Hz standard servo signal
#define LEDC_PWM_RES_SERVO     16       // 16-bit resolution for precise pulse width

// System Diagnostics & Modes
#define HARDWARE_MODE           true
#define SIMULATION_MODE         false

#endif // CONFIG_H
