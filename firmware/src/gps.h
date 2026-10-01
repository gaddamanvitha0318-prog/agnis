#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include "pins.h"
#include "types.h"

class GPSSystem {
public:
    GPSSystem();
    void init();
    void update();

    bool hasFix() const { return fixStatus; }
    int getSatellites() const { return satelliteCount; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    float getSpeedKmh() const { return speedKmh; }
    DeviceHealth getHealth() const { return health; }

private:
    HardwareSerial gpsSerial;
    TinyGPSPlus gpsParser;
    bool fixStatus;
    int satelliteCount;
    double latitude;
    double longitude;
    float speedKmh;
    DeviceHealth health;
    unsigned long lastValidTime;
};

extern GPSSystem gpsSystem;

#endif // GPS_H
