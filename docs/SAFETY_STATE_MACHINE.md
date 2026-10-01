# AGNIS-4WD — Safety State Machine Specification

## Priority Hierarchy
```
  +-------------------------------------------------------------+
  |  1. PHYSICAL E-STOP (Hardware Cutoff & Software State)       |
  +-------------------------------------------------------------+
  |  2. CRITICAL HARDWARE FAULT (I2C Bus / Sensor Lockup)       |
  +-------------------------------------------------------------+
  |  3. OBSTACLE EMERGENCY (< 20 cm Clearance)                 |
  +-------------------------------------------------------------+
  |  4. COMMUNICATION LOSS (Heartbeat Timeout > 750 ms)        |
  +-------------------------------------------------------------+
  |  5. CRITICAL BATTERY (< 9.9 V Cutoff)                      |
  +-------------------------------------------------------------+
  |  6. NORMAL OPERATING COMMANDS (Manual / Assist / Auto)      |
  +-------------------------------------------------------------+
```

## State Definitions & Indicator Matrix

| Safety State | Condition | Motors | Green LED | Blue LED | Red LED | Buzzer |
|---|---|---|---|---|---|---|
| `BOOT` | System powering on | Disabled | OFF | OFF | ON | Silent |
| `SELF_TEST` | Running sensor diagnostics | Disabled | Flashing | OFF | Flashing | Short Beep |
| `READY` | Self-test passed, standby | Disabled | ON | OFF | OFF | Silent |
| `MANUAL` | Remote joystick active | Active | ON | OFF | OFF | Silent |
| `ASSIST` | Active collision protection | Active | OFF | ON | OFF | Silent |
| `AUTO` | Autonomous obstacle sweep | Active | OFF | ON | OFF | Silent |
| `WARNING` | Obstacle 40-80 cm | Active (Capped) | ON | OFF | ON | Slow Beep (800ms) |
| `BRAKING` | Obstacle 20-40 cm | Decelerating | OFF | OFF | ON | Fast Beep (300ms) |
| `EMERGENCY_STOP` | E-stop pressed or <20cm | **DISABLED** | OFF | OFF | ON | Rapid Beep (100ms) |
| `COMMUNICATION_LOSS` | No command for >750ms | **STOPPED** | OFF | OFF | ON | Periodic Beep (500ms)|
| `CRITICAL_BATTERY` | Voltage < 9.9V | **STOPPED** | OFF | OFF | ON | Continuous Beep |
