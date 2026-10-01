#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

class WiFiNetworkManager {
public:
    WiFiNetworkManager();
    void init();
    String getIPAddress() const;

private:
    IPAddress apIP;
};

extern WiFiNetworkManager wifiManager;

#endif // WIFI_MANAGER_H
