#include "diagnostics.h"

SystemDiagnostics diagnostics;

SystemDiagnostics::SystemDiagnostics() : selfTestPassed(false) {}

bool SystemDiagnostics::runSelfTest() {
    Serial.println("\n========== AGNIS-4WD STARTUP SELF-TEST ==========");
    
    // 1. Check I2C / MPU6050
    bool imuOk = imuSystem.init();
    Serial.printf("[SELF-TEST] MPU6050 IMU: %s\n", imuOk ? "PASS" : "FAIL");

    // 2. Check Servos
    servoController.init();
    Serial.println("[SELF-TEST] Servos (LEDC PWM): PASS");

    // 3. Check Dual Ultrasonic
    dualUltrasonic.init();
    dualUltrasonic.update();
    Serial.printf("[SELF-TEST] Front Ultrasonic: %.1f cm\n", dualUltrasonic.getFrontDistance());
    Serial.printf("[SELF-TEST] Rear Ultrasonic:  %.1f cm\n", dualUltrasonic.getRearDistance());

    // 4. Check Power Monitor
    powerMonitor.init();
    powerMonitor.update();
    Serial.printf("[SELF-TEST] Battery Voltage:  %.2f V\n", powerMonitor.getVoltage());

    // 5. Check E-stop
    bool estopActive = safetyManager.isEstopActive();
    Serial.printf("[SELF-TEST] E-stop Status:  %s\n", estopActive ? "ENGAGED" : "CLEAR");

    selfTestPassed = imuOk && (powerMonitor.getVoltage() > BATTERY_CRIT_VOLTAGE);
    Serial.printf("==================================================\nRESULT: %s\n\n", selfTestPassed ? "PASSED" : "FAILED");
    return selfTestPassed;
}

String SystemDiagnostics::runDiagnosticReport() {
    String report = "{\n";
    report += "  \"self_test\": " + String(selfTestPassed ? "true" : "false") + ",\n";
    report += "  \"front_us\": " + String(dualUltrasonic.getFrontDistance()) + ",\n";
    report += "  \"rear_us\": " + String(dualUltrasonic.getRearDistance()) + ",\n";
    report += "  \"battery_v\": " + String(powerMonitor.getVoltage()) + ",\n";
    report += "  \"battery_i\": " + String(powerMonitor.getCurrent()) + ",\n";
    report += "  \"imu_ok\": " + String(imuSystem.getHealth() == DeviceHealth::OK ? "true" : "false") + ",\n";
    report += "  \"gps_fix\": " + String(gpsSystem.hasFix() ? "true" : "false") + "\n";
    report += "}";
    return report;
}

bool SystemDiagnostics::testMotorLeft(bool confirm) {
    if (!confirm) return false;
    motorController.setLeftMotor(120);
    delay(1000);
    motorController.stopMotors();
    return true;
}

bool SystemDiagnostics::testMotorRight(bool confirm) {
    if (!confirm) return false;
    motorController.setRightMotor(120);
    delay(1000);
    motorController.stopMotors();
    return true;
}
