#include "web_server.h"

WebServerManager webServerManager;

WebServerManager::WebServerManager() : server(80), ws("/ws") {}

void WebServerManager::init() {
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onWsEvent(server, client, type, arg, data, len);
    });
    server.addHandler(&ws);

    setupRoutes();
    server.begin();
    Serial.println("[WEB] Async HTTP and WebSocket server started on port 80");
}

void WebServerManager::setupRoutes() {
    // REST API Endpoint: /api/status
    server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        JsonDocument doc;
        doc["mode"] = (int)safetyManager.getMode();
        doc["state"] = (int)safetyManager.getState();
        doc["estop"] = safetyManager.isEstopActive();
        doc["uptime"] = millis();
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    // REST API Endpoint: /api/drive (POST with JSON speed & turn)
    server.on("/api/drive", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, data, len);
        if (!err) {
            int speed = doc["speed"] | 0;
            int turn = doc["turn"] | 0;
            safetyManager.updateCommandHeartbeat();
            
            if (safetyManager.getMode() != OperatingMode::AUTO) {
                motorController.drive(speed, turn);
            }
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"error\":\"invalid json\"}");
        }
    });

    // REST API Endpoint: /api/mode (POST new mode)
    server.on("/api/mode", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len)) {
            String m = doc["mode"] | "MANUAL";
            if (m == "MANUAL") safetyManager.setMode(OperatingMode::MANUAL);
            else if (m == "ASSIST") safetyManager.setMode(OperatingMode::ASSIST);
            else if (m == "AUTO") safetyManager.setMode(OperatingMode::AUTO);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"error\":\"invalid payload\"}");
        }
    });

    // REST API Endpoint: /api/estop (POST software E-stop trigger/reset)
    server.on("/api/estop", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        JsonDocument doc;
        if (!deserializeJson(doc, data, len)) {
            bool action = doc["action"] | true;
            if (action) {
                safetyManager.triggerSoftwareEstop();
            } else {
                safetyManager.resetEstop();
            }
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"error\":\"invalid payload\"}");
        }
    });

    // REST API Endpoint: /api/events
    server.on("/api/events", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", eventLogger.getEventsJson());
    });
}

void WebServerManager::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("[WS] Client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("[WS] Client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            handleWebSocketMessage(arg, data, len);
        }
    }
}

void WebServerManager::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, data, len);
    if (err) return;

    String type = doc["type"] | "";
    if (type == "drive") {
        int speed = doc["speed"] | 0;
        int turn = doc["turn"] | 0;
        safetyManager.updateCommandHeartbeat();
        if (safetyManager.getMode() != OperatingMode::AUTO) {
            motorController.drive(speed, turn);
        }
    } else if (type == "estop") {
        safetyManager.triggerSoftwareEstop();
    } else if (type == "mode") {
        String m = doc["mode"] | "MANUAL";
        if (m == "MANUAL") safetyManager.setMode(OperatingMode::MANUAL);
        else if (m == "ASSIST") safetyManager.setMode(OperatingMode::ASSIST);
        else if (m == "AUTO") safetyManager.setMode(OperatingMode::AUTO);
    }
}

void WebServerManager::broadcastTelemetry() {
    if (ws.count() == 0) return;

    JsonDocument doc;
    doc["t"] = millis();
    doc["v"] = powerMonitor.getVoltage();
    doc["i"] = powerMonitor.getCurrent();
    doc["p"] = powerMonitor.getPower();
    doc["soc"] = powerMonitor.getEstimatedSOC();
    doc["df"] = dualUltrasonic.getFrontDistance();
    doc["dr"] = dualUltrasonic.getRearDistance();
    doc["roll"] = imuSystem.getRoll();
    doc["pitch"] = imuSystem.getPitch();
    doc["gps_fix"] = gpsSystem.hasFix();
    doc["sats"] = gpsSystem.getSatellites();
    doc["lat"] = gpsSystem.getLatitude();
    doc["lng"] = gpsSystem.getLongitude();
    doc["mode"] = (int)safetyManager.getMode();
    doc["state"] = (int)safetyManager.getState();
    doc["estop"] = safetyManager.isEstopActive();
    doc["lspeed"] = motorController.getLeftSpeed();
    doc["rspeed"] = motorController.getRightSpeed();

    String output;
    serializeJson(doc, output);
    ws.textAll(output);
}
