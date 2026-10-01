/*
 ============================================================================
  AGNIS-4WD — STANDALONE TANK-STEERING MOTOR DRIVER TEST (ESP32 + L298N)
 ============================================================================
  
  Tank Turning (Skid Steering Zero-Radius Pivot):
    - TANK TURN LEFT:  Left Side Motors turn BACKWARDS (-Speed),
                       Right Side Motors turn FORWARDS (+Speed).
    - TANK TURN RIGHT: Left Side Motors turn FORWARDS (+Speed),
                       Right Side Motors turn BACKWARDS (-Speed).

  Pinout Mapping (ESP32 DevKit V1 -> L298N):
  ------------------------------------------------------------
  L298N Channel A (Left Side Motors: Front Left + Rear Left):
    - ENA (PWM Speed):   GPIO 14  (LEDC Channel 0)
    - IN1 (Direction 1): GPIO 27
    - IN2 (Direction 2): GPIO 26

  L298N Channel B (Right Side Motors: Front Right + Rear Right):
    - ENB (PWM Speed):   GPIO 32  (LEDC Channel 1)
    - IN3 (Direction 1): GPIO 25
    - IN4 (Direction 2): GPIO 33

  Common Connections:
    - ESP32 GND connected to L298N GND and Battery Power GND!
 ============================================================================
*/

#include <Arduino.h>

// GPIO Definitions
#define PIN_ENA 14
#define PIN_IN1 27
#define PIN_IN2 26

#define PIN_ENB 32
#define PIN_IN3 25
#define PIN_IN4 33

// PWM Setup Constants
#define PWM_FREQ 5000
#define PWM_RES  8     // 8-bit resolution (0 - 255)
#define CH_LEFT  0
#define CH_RIGHT 1

// Speed Levels
#define SPEED_SLOW 130
#define SPEED_FAST 240
#define SPEED_TANK 200

void setLeftMotor(int speed) {
    if (speed > 0) {
        digitalWrite(PIN_IN1, HIGH);
        digitalWrite(PIN_IN2, LOW);
        ledcWrite(CH_LEFT, speed);
    } else if (speed < 0) {
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, HIGH);
        ledcWrite(CH_LEFT, -speed);
    } else {
        digitalWrite(PIN_IN1, LOW);
        digitalWrite(PIN_IN2, LOW);
        ledcWrite(CH_LEFT, 0);
    }
}

void setRightMotor(int speed) {
    if (speed > 0) {
        digitalWrite(PIN_IN3, HIGH);
        digitalWrite(PIN_IN4, LOW);
        ledcWrite(CH_RIGHT, speed);
    } else if (speed < 0) {
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, HIGH);
        ledcWrite(CH_RIGHT, -speed);
    } else {
        digitalWrite(PIN_IN3, LOW);
        digitalWrite(PIN_IN4, LOW);
        ledcWrite(CH_RIGHT, 0);
    }
}

void stopMotors() {
    setLeftMotor(0);
    setRightMotor(0);
}

// Dedicated Tank Turn Functions
void tankTurnLeft(int speed) {
    // Left side BACKWARD (-speed), Right side FORWARD (+speed)
    setLeftMotor(-speed);
    setRightMotor(speed);
}

void tankTurnRight(int speed) {
    // Left side FORWARD (+speed), Right side BACKWARD (-speed)
    setLeftMotor(speed);
    setRightMotor(-speed);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n==================================================");
    Serial.println("  AGNIS-4WD TANK-STEERING MOTOR DRIVER TESTER");
    Serial.println("==================================================");

    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_IN3, OUTPUT);
    pinMode(PIN_IN4, OUTPUT);

    ledcSetup(CH_LEFT, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_ENA, CH_LEFT);

    ledcSetup(CH_RIGHT, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_ENB, CH_RIGHT);

    stopMotors();
    Serial.println("[TEST READY] Starting Tank-Steering sequence in 3 seconds...");
    delay(3000);
}

void loop() {
    // 1. FORWARD SLOW
    Serial.println("\n>>> [1] FORWARD SLOW (Speed 130)");
    setLeftMotor(SPEED_SLOW);
    setRightMotor(SPEED_SLOW);
    delay(2000);

    stopMotors();
    delay(1000);

    // 2. FORWARD FAST
    Serial.println("\n>>> [2] FORWARD FAST (Speed 240)");
    setLeftMotor(SPEED_FAST);
    setRightMotor(SPEED_FAST);
    delay(2000);

    stopMotors();
    delay(1000);

    // 3. BACKWARD SLOW
    Serial.println("\n>>> [3] BACKWARD SLOW (Speed -130)");
    setLeftMotor(-SPEED_SLOW);
    setRightMotor(-SPEED_SLOW);
    delay(2000);

    stopMotors();
    delay(1000);

    // 4. BACKWARD FAST
    Serial.println("\n>>> [4] BACKWARD FAST (Speed -240)");
    setLeftMotor(-SPEED_FAST);
    setRightMotor(-SPEED_FAST);
    delay(2000);

    stopMotors();
    delay(1000);

    // 5. TANK TURN LEFT (Left Tyres BACKWARD -200, Right Tyres FORWARD +200)
    Serial.println("\n>>> [5] TANK TURN LEFT (Left: BACKWARDS -200 | Right: FORWARDS +200)");
    tankTurnLeft(SPEED_TANK);
    delay(3000);

    stopMotors();
    delay(1500);

    // 6. TANK TURN RIGHT (Left Tyres FORWARD +200, Right Tyres BACKWARD -200)
    Serial.println("\n>>> [6] TANK TURN RIGHT (Left: FORWARDS +200 | Right: BACKWARDS -200)");
    tankTurnRight(SPEED_TANK);
    delay(3000);

    stopMotors();
    Serial.println("\n==================================================");
    Serial.println("  SEQUENCE COMPLETE. REPEATING IN 4 SECONDS       ");
    Serial.println("==================================================");
    delay(4000);
}
