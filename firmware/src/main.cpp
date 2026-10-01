#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "types.h"
#include "motor_controller.h"
#include "safety_manager.h"
#include "ultrasonic.h"
#include "servo_controller.h"
#include "imu.h"
#include "gps.h"
#include "power_monitor.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "telemetry.h"
#include "event_logger.h"
#include "autonomous.h"
#include "diagnostics.h"

unsigned long lastWsBroadcastTime = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n========================================================");
    Serial.println("  AGNIS-4WD SMART AUTONOMOUS SAFETY VEHICLE FIRMWARE");
    Serial.println("========================================================");

    // 1. Core Hardware Initializations
    motorController.init();
    safetyManager.init();
    dualUltrasonic.init();
    servoController.init();
    powerMonitor.init();
    gpsSystem.init();

    // 2. I2C Bus & Sensor Diagnostics
    diagnostics.runSelfTest();

    // 3. Autonomous & Event Logger Init
    autoNav.init();
    eventLogger.log("BOOT", "AGNIS-4WD initialized successfully");

    // 4. Wi-Fi SoftAP & Async Web Server
    wifiManager.init();
    webServerManager.init();

    Serial.println("[SYSTEM] Setup complete. Engine ready in MANUAL mode.");
}

void loop() {
    // High Priority Subsystems
    safetyManager.update();
    motorController.updateRamp();

    // Periodic Subsystems
    dualUltrasonic.update();
    servoController.updateSweep();
    imuSystem.update();
    gpsSystem.update();
    powerMonitor.update();
    telemetrySystem.update();

    // Autonomous Control Task
    if (safetyManager.getMode() == OperatingMode::AUTO) {
        autoNav.update();
    }

    // Telemetry Broadcast to WebSocket clients (10 Hz)
    if (millis() - lastWsBroadcastTime >= 100) {
        lastWsBroadcastTime = millis();
        webServerManager.broadcastTelemetry();
    }
}
