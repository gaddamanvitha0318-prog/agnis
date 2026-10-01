#include "event_logger.h"

EventLogger eventLogger;

EventLogger::EventLogger() : head(0), count(0) {}

void EventLogger::log(const String& type, const String& message) {
    buffer[head].timestamp = millis();
    buffer[head].type = type;
    buffer[head].message = message;

    head = (head + 1) % MAX_EVENTS;
    if (count < MAX_EVENTS) {
        count++;
    }

    Serial.printf("[%lu] [%s] %s\n", millis(), type.c_str(), message.c_str());
}

EventLog EventLogger::getEvent(int index) const {
    if (index < 0 || index >= count) {
        return {0, "", ""};
    }
    int readIdx = (head - count + index + MAX_EVENTS) % MAX_EVENTS;
    return buffer[readIdx];
}

String EventLogger::getEventsJson() const {
    String json = "[";
    for (int i = 0; i < count; i++) {
        EventLog ev = getEvent(i);
        json += "{\"t\":" + String(ev.timestamp) + ",\"type\":\"" + ev.type + "\",\"msg\":\"" + ev.message + "\"}";
        if (i < count - 1) json += ",";
    }
    json += "]";
    return json;
}
