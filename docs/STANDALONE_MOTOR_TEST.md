# AGNIS-4WD — Standalone Motor Driver Direction Test Guide

This standalone test program allows testing the ESP32 DevKit V1 and L298N H-bridge motor driver wiring and directions without loading full sensors or Wi-Fi code.

## Quick Wiring Checklist
1. **L298N Channel A (Left Motors)**:
   - `ENA` -> ESP32 **GPIO 14**
   - `IN1` -> ESP32 **GPIO 27**
   - `IN2` -> ESP32 **GPIO 26**
2. **L298N Channel B (Right Motors)**:
   - `ENB` -> ESP32 **GPIO 32**
   - `IN3` -> ESP32 **GPIO 25**
   - `IN4` -> ESP32 **GPIO 33**
3. **Common Ground**:
   - ESP32 `GND` **MUST** be connected to L298N `GND` and Motor Battery Power `GND`!

## How to Flash and Test via PlatformIO
1. Run the upload command from the project root:
   ```bash
   /home/siddhartha/.local/bin/pio run -e esp32dev -t upload
   ```
2. Open the serial monitor at 115200 baud to observe output:
   ```bash
   /home/siddhartha/.local/bin/pio device monitor -b 115200
   ```

## Test Sequence Execution
The test routine automatically cycles through 6 motion patterns:
1. **FORWARD SLOW** (PWM 130) for 2.0 sec
2. **FORWARD FAST** (PWM 240) for 2.0 sec
3. **BACKWARD SLOW** (PWM -130) for 2.0 sec
4. **BACKWARD FAST** (PWM -240) for 2.0 sec
5. **TANK TURN LEFT (Pivot Left)** (Left Tyres BACKWARD -200, Right Tyres FORWARD +200) for 3.0 sec
6. **TANK TURN RIGHT (Pivot Right)** (Left Tyres FORWARD +200, Right Tyres BACKWARD -200) for 3.0 sec
