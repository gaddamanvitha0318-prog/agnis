# AGNIS-4WD — Smart Autonomous 4WD Safety Vehicle

AGNIS-4WD is a production-grade embedded autonomous safety vehicle platform developed on the **ESP32 DevKit V1 (ESP-WROOM-32)**. It features 4WD differential motor control, dual ultrasonic sweep scanning via direct ESP32 hardware PWM (LEDC), MPU6050 telemetry, NEO-6M GPS, power monitoring, double-layered physical E-stop safety, and a responsive Web-based 3-Phone Remote Control & Engineering Telemetry Dashboard.

---

## Key Hardware Architecture
- **Main Controller**: ESP32 DevKit V1 (30-pin ESP-WROOM-32)
- **Drive Subsystem**: 4 × BO DC Geared Motors driven by 1 × L298N Dual H-Bridge Module
- **Obstacle Sensing**: 2 × HC-SR04 Ultrasonic Sensors (Front & Rear with 1k/2k resistor dividers)
- **Servo Scanning**: 2 × SG90 9g Servos driven directly by ESP32 LEDC PWM (GPIO 2 & GPIO 15)
- **Telemetry Sensors**: MPU6050 6-Axis IMU (I2C) & NEO-6M GPS Module (UART2)
- **Camera Subsystem**: Standalone ESP32-CAM delivering MJPEG stream over Wi-Fi
- **Safety**: Physical Normally-Closed Mushroom E-stop Button + Software Safety Manager State Machine
- **Indicators**: RGB Status LEDs (Green, Blue, Red) & Active Audible Buzzer

---

## Quick Start — Standalone Motor Driver Test
To test motor wiring, directions, and speeds (Forward fast/slow, Reverse fast/slow, Left/Right turns, Pivots):

1. Connect ESP32 DevKit V1 to laptop via USB (`/dev/ttyACM0`).
2. Run PlatformIO build & upload command:
   ```bash
   /home/siddhartha/.local/bin/pio run -t upload
   ```
3. Open serial monitor:
   ```bash
   /home/siddhartha/.local/bin/pio device monitor -b 115200
   ```

---

## Operating Modes
1. **MANUAL**: Direct remote control from Phone 2 joystick with active E-stop & comms timeout protection.
2. **ASSIST**: Driver steering with automatic collision prevention, speed capping, and forced braking.
3. **AUTO**: ESP32-governed autonomous obstacle avoidance using sweep scan clearance analysis.

---

## Three-Phone Web Dashboard Architecture
Connect smartphone devices to the `AGNIS-4WD` Wi-Fi Access Point (Password: `AGNIS2026`) and navigate to `http://192.168.4.1/`:
- **Phone 1 (Vision)**: `vision.html` — Real-time camera feed & stream status
- **Phone 2 (Drive)**: `drive.html` — Touch joystick & E-stop controller
- **Phone 3 (Telemetry)**: `telemetry.html` — Live power, IMU, GPS, and system event logs
- **Commissioning**: `diagnostics.html` — Individual subsystem hardware testing

---

## Documentation Index
- [FINAL_GPIO_MAP.md](docs/FINAL_GPIO_MAP.md) — Pin assignments & logic levels
- [POWER_ARCHITECTURE.md](docs/POWER_ARCHITECTURE.md) — Rail wiring & voltage regulators
- [SAFETY_STATE_MACHINE.md](docs/SAFETY_STATE_MACHINE.md) — Hierarchy, states & indicators
- [STANDALONE_MOTOR_TEST.md](docs/STANDALONE_MOTOR_TEST.md) — Standalone motor driver test guide
