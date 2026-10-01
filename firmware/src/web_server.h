#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include "types.h"
#include "safety_manager.h"
#include "motor_controller.h"
#include "ultrasonic.h"
#include "imu.h"
#include "gps.h"
#include "power_monitor.h"
#include "event_logger.h"

class WebServerManager {
public:
    WebServerManager();
    void init();
    void broadcastTelemetry();

private:
    AsyncWebServer server;
    AsyncWebSocket ws;

    void setupRoutes();
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
};

extern WebServerManager webServerManager;

#endif // WEB_SERVER_H
