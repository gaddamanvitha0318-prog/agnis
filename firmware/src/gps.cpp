#include "gps.h"

GPSSystem gpsSystem;

GPSSystem::GPSSystem()
    : gpsSerial(2), fixStatus(false), satelliteCount(0),
      latitude(0.0), longitude(0.0), speedKmh(0.0f),
      health(DeviceHealth::UNKNOWN), lastValidTime(0) {}

void GPSSystem::init() {
    gpsSerial.begin(9600, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);
}

void GPSSystem::update() {
    while (gpsSerial.available() > 0) {
        gpsParser.encode(gpsSerial.read());
    }

    if (gpsParser.location.isValid()) {
        latitude = gpsParser.location.lat();
        longitude = gpsParser.location.lng();
        fixStatus = true;
        lastValidTime = millis();
    } else {
        if (millis() - lastValidTime > 5000) {
            fixStatus = false;
        }
    }

    if (gpsParser.satellites.isValid()) {
        satelliteCount = gpsParser.satellites.value();
    }

    if (gpsParser.speed.isValid()) {
        speedKmh = gpsParser.speed.kmph();
    }

    if (millis() - lastValidTime < 3000) {
        health = DeviceHealth::OK;
    } else if (millis() - lastValidTime < 10000) {
        health = DeviceHealth::WARNING;
    } else {
        health = DeviceHealth::FAULT;
    }
}
