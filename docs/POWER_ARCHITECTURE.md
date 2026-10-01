# AGNIS-4WD — Power Architecture & Rail Wiring

```
                     +----------------------------------------+
                     | 3S Li-Ion Battery Pack (11.1V - 12.6V) |
                     +----------------------------------------+
                                         |
                                         |--- Main Switch & Fuse (10A)
                                         |
                     +-------------------+-------------------+
                     |                                       |
                     v                                       v
         +------------------------+             +------------------------+
         | High-Current Buck Reg  |             |  L298N Dual H-Bridge   |
         | (5V 5A Output Rail)    |             |  Motor Supply (VS)     |
         +------------------------+             +------------------------+
                     |                                       |
    +----------------+----------------+                      v
    |                |                |           +--------------------+
    v                v                v           | 4× BO Geared       |
+-------+        +-------+        +-------+       | DC Motors          |
| ESP32 |        | SG90  |        |HC-SR04|       +--------------------+
| 5V    |        |Servos |        | 5V VCC|
+-------+        +-------+        +-------+

           GND BUS: COMMON GROUND FOR ALL MODULES AND POWER RAILS
```

## Power Rail Specifications
1. **Motor Supply Rail (VS)**:
   - Direct connection from battery pack through Emergency Stop physical relay / hardware cutoff.
   - Operating Voltage: 11.1V - 12.6V DC.
2. **Logic & Servo Supply Rail (5V)**:
   - Stepped down from 12V via dedicated high-efficiency DC-DC Buck Converter (5V 5A).
   - Supplies power to ESP32 VIN pin, HC-SR04 $V_{CC}$ (5V), and SG90 Servos $V_{CC}$ (5V).
3. **Internal ESP32 3.3V Rail**:
   - Internal ESP32 LDO regulator powers ESP32 micro-controller core, MPU6050 (3.3V), and NEO-6M GPS (3.3V).
   - **NEVER power high-current servos from the ESP32 3.3V rail!**

## Protection Components
- **Double-Layer E-Stop**: The mushroom E-stop button physically disconnects the L298N motor power line AND signals GPIO 19 on the ESP32.
- **HC-SR04 Level Protection**: 5V Echo signals are stepped down using 1kΩ / 2kΩ resistor dividers before entering ESP32 GPIO 34 and GPIO 35.
