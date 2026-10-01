#ifndef PINS_H
#define PINS_H

// ============================================================================
// AGNIS-4WD — FINAL GPIO MAP (ESP32 DevKit V1 / ESP-WROOM-32 30-Pin)
// ============================================================================

// --- L298N MOTOR DRIVER ---
#define PIN_MOTOR_ENA   14  // Left Motors Speed (PWM LEDC Channel 0)
#define PIN_MOTOR_IN1   27  // Left Motors Direction 1
#define PIN_MOTOR_IN2   26  // Left Motors Direction 2
#define PIN_MOTOR_ENB   32  // Right Motors Speed (PWM LEDC Channel 1)
#define PIN_MOTOR_IN3   25  // Right Motors Direction 1
#define PIN_MOTOR_IN4   33  // Right Motors Direction 2

// --- SERVO SYSTEM (Direct ESP32 Hardware PWM LEDC) ---
#define PIN_SERVO_FRONT  2  // Front Ultrasonic Scanner Servo (LEDC Channel 2)
#define PIN_SERVO_REAR  15  // Rear Ultrasonic Scanner Servo (LEDC Channel 3)

// --- I2C BUS (MPU6050 IMU) ---
#define PIN_I2C_SDA     21  // Hardware I2C SDA
#define PIN_I2C_SCL     22  // Hardware I2C SCL

// --- GPS MODULE (NEO-6M UART2) ---
#define PIN_GPS_RX      16  // ESP32 RX2 connected to NEO-6M TX
#define PIN_GPS_TX      17  // ESP32 TX2 connected to NEO-6M RX

// --- HC-SR04 ULTRASONIC SENSORS ---
#define PIN_FRONT_TRIG  13  // Front Ultrasonic Trigger Output
#define PIN_FRONT_ECHO  34  // Front Ultrasonic Echo Input (Input Only, 1k/2k Divider)
#define PIN_REAR_TRIG    4  // Rear Ultrasonic Trigger Output
#define PIN_REAR_ECHO   35  // Rear Ultrasonic Echo Input (Input Only, 1k/2k Divider)

// --- PHYSICAL E-STOP ---
#define PIN_ESTOP_SENSE 19  // Normally-Closed (NC) E-Stop Sense (Pull-Up, Active HIGH when open)

// --- POWER TELEMETRY ADC ---
#define PIN_VOLTAGE_ADC 36  // Battery Voltage Divider ADC (VP / ADC1_CH0)
#define PIN_CURRENT_ADC 39  // Current Sensor ADC (VN / ADC1_CH3)

// --- STATUS INDICATORS ---
#define PIN_LED_GREEN   18  // System Ready / Normal Indicator
#define PIN_LED_BLUE    23  // Active Mode (Assist/Auto) Indicator
#define PIN_LED_RED      5  // Warning / E-Stop / Fault Indicator
#define PIN_BUZZER      12  // Audible Alarm Buzzer

#endif // PINS_H
