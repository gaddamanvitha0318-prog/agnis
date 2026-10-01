#include "wifi_manager.h"

WiFiNetworkManager wifiManager;

WiFiNetworkManager::WiFiNetworkManager() : apIP(192, 168, 4, 1) {}

void WiFiNetworkManager::init() {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS, WIFI_AP_CHANNEL, 0, WIFI_MAX_CLIENTS);
    Serial.printf("[WIFI] SoftAP started. SSID: %s, IP: %s\n", WIFI_AP_SSID, getIPAddress().c_str());
}

String WiFiNetworkManager::getIPAddress() const {
    return WiFi.softAPIP().toString();
}
