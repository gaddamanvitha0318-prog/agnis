/*
 ============================================================================
  AGNIS-4WD — STANDALONE SERVO 90° CALIBRATION PROGRAM (ESP32 DevKit V1)
 ============================================================================

  Servo GPIO Connections:
    - Front Ultrasonic Scanner Servo: GPIO 2  (LEDC Channel 2)
    - Rear Ultrasonic Scanner Servo:  GPIO 15 (LEDC Channel 3)

  Power Safety Rules:
    - Servos MUST be powered from an external 5V regulated rail!
    - ESP32 GND MUST be connected to 5V Regulator GND!
    - Do NOT power SG90 servos directly from ESP32 3.3V pin.

  Calibration Instructions:
    1. Upload this code to the ESP32.
    2. The servos will automatically move to exact 90° CENTER position.
    3. Press/mount your physical servo horn pointing DEAD STRAIGHT ahead.
 ============================================================================
*/

#include <Arduino.h>

// Servo GPIO Definitions
#define PIN_SERVO_FRONT 2
#define PIN_SERVO_REAR  15

// LEDC Setup for 50 Hz Servo PWM (16-bit resolution: 0 - 65535)
#define LEDC_FREQ      50
#define LEDC_RES       16
#define CH_SERVO_FRONT 2
#define CH_SERVO_REAR  3

// Servo Pulse Duty Cycle Constants for 16-bit resolution (50 Hz / 20ms period)
// 0.5ms pulse (0 deg)   -> 1638 duty
// 1.5ms pulse (90 deg)  -> 4915 duty (EXACT 90 DEGREE CENTER)
// 2.5ms pulse (180 deg) -> 8192 duty
uint32_t angleToDuty(int angle) {
    angle = constrain(angle, 0, 180);
    return 1638 + ((uint32_t)angle * (8192 - 1638) / 180);
}

void setServoFront(int angle) {
    ledcWrite(CH_SERVO_FRONT, angleToDuty(angle));
}

void setServoRear(int angle) {
    ledcWrite(CH_SERVO_REAR, angleToDuty(angle));
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n========================================================");
    Serial.println("  AGNIS-4WD STANDALONE SERVO 90° CALIBRATION SYSTEM");
    Serial.println("========================================================");

    // Setup LEDC PWM Channels
    ledcSetup(CH_SERVO_FRONT, LEDC_FREQ, LEDC_RES);
    ledcAttachPin(PIN_SERVO_FRONT, CH_SERVO_FRONT);

    ledcSetup(CH_SERVO_REAR, LEDC_FREQ, LEDC_RES);
    ledcAttachPin(PIN_SERVO_REAR, CH_SERVO_REAR);

    // IMMEDIATELY LOCK SERVOS AT 90 DEGREES
    setServoFront(90);
    setServoRear(90);

    Serial.println("\n [90° CENTER LOCK ENGAGED]");
    Serial.println(" -> Front Servo (GPIO 2) locked at 90°");
    Serial.println(" -> Rear Servo  (GPIO 15) locked at 90°");
    Serial.println("\n>>> MOUNT SERVO HORNS STRAIGHT AHEAD NOW! <<<");
    delay(4000);
}

void loop() {
    // 1. HOLD AT 90° CENTER FOR CALIBRATION
    Serial.println("\n>>> POSITION: 90° CENTER (Alignment Position) <<<");
    setServoFront(90);
    setServoRear(90);
    delay(5000);

    // 2. TEST MINIMUM SWEEP (30° LEFT)
    Serial.println(">>> TESTING MIN ANGLE: 30° (Left Sweep) <<<");
    setServoFront(30);
    setServoRear(150); // Opposite phase for rear
    delay(2500);

    // 3. RETURN TO 90° CENTER
    Serial.println(">>> POSITION: 90° CENTER <<<");
    setServoFront(90);
    setServoRear(90);
    delay(2000);

    // 4. TEST MAXIMUM SWEEP (150° RIGHT)
    Serial.println(">>> TESTING MAX ANGLE: 150° (Right Sweep) <<<");
    setServoFront(150);
    setServoRear(30);
    delay(2500);

    // 5. RETURN TO 90° CENTER
    Serial.println(">>> POSITION: 90° CENTER <<<");
    setServoFront(90);
    setServoRear(90);
    delay(3000);
}
