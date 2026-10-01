# AGNIS-4WD — Final Hardware GPIO Pin Mapping

Controller: **ESP32 DevKit V1 (ESP-WROOM-32 30-Pin)**

| Function | GPIO | Direction | Logic Voltage | Connected Device | Notes |
|---|---|---|---|---|---|
| Left Motor PWM (ENA) | GPIO 14 | Output | 3.3V | L298N ENA Pin | LEDC Channel 0 (5 kHz PWM) |
| Left Motor IN1 | GPIO 27 | Output | 3.3V | L298N IN1 Pin | Direction Control 1 |
| Left Motor IN2 | GPIO 26 | Output | 3.3V | L298N IN2 Pin | Direction Control 2 |
| Right Motor PWM (ENB)| GPIO 32 | Output | 3.3V | L298N ENB Pin | LEDC Channel 1 (5 kHz PWM) |
| Right Motor IN3 | GPIO 25 | Output | 3.3V | L298N IN3 Pin | Direction Control 1 |
| Right Motor IN4 | GPIO 33 | Output | 3.3V | L298N IN4 Pin | Direction Control 2 |
| Front Servo Signal | GPIO 2 | Output | 3.3V PWM | SG90 Front Servo | LEDC Channel 2 (50 Hz Servo) |
| Rear Servo Signal | GPIO 15 | Output | 3.3V PWM | SG90 Rear Servo | LEDC Channel 3 (50 Hz Servo) |
| I2C SDA | GPIO 21 | Bidirectional | 3.3V | MPU6050 IMU | Hardware I2C SDA |
| I2C SCL | GPIO 22 | Output | 3.3V | MPU6050 IMU | Hardware I2C SCL |
| GPS RX2 | GPIO 16 | Input | 3.3V | NEO-6M TX | Hardware UART2 RX |
| GPS TX2 | GPIO 17 | Output | 3.3V | NEO-6M RX | Hardware UART2 TX |
| Front Trig | GPIO 13 | Output | 3.3V | HC-SR04 Front Trig | 10µs trigger pulse |
| Front Echo | GPIO 34 | Input | 3.3V max | HC-SR04 Front Echo | **Input Only Pin** via 1k/2k Divider |
| Rear Trig | GPIO 4 | Output | 3.3V | HC-SR04 Rear Trig | 10µs trigger pulse |
| Rear Echo | GPIO 35 | Input | 3.3V max | HC-SR04 Rear Echo | **Input Only Pin** via 1k/2k Divider |
| Physical E-Stop Sense| GPIO 19 | Input | 3.3V | NC Mushroom Button | Internal Pull-Up; Active HIGH on open |
| Voltage ADC | GPIO 36 | Input | 3.3V max | Voltage Divider | ADC1_CH0 (VP) |
| Current ADC | GPIO 39 | Input | 3.3V max | ACS712 Current Sensor | ADC1_CH3 (VN) |
| Green Status LED | GPIO 18 | Output | 3.3V | Status LED (Green) | Ready / Normal |
| Blue Status LED | GPIO 23 | Output | 3.3V | Status LED (Blue) | Active Assist / Auto Mode |
| Red Status LED | GPIO 5 | Output | 3.3V | Status LED (Red) | Warning / Emergency / Fault |
| Buzzer | GPIO 12 | Output | 3.3V | Active Buzzer | Non-blocking tone patterns |

> **IMPORTANT**: ESP32-CAM uses its own onboard ESP32 controller and GPIO layout (e.g. GPIO 32 pwdn, GPIO 0 xclk). It operates as a separate standalone subsystem over Wi-Fi.
