#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <Arduino.h>
#include "types.h"

#define MAX_EVENTS 50

class EventLogger {
public:
    EventLogger();
    void log(const String& type, const String& message);
    int getEventCount() const { return count; }
    EventLog getEvent(int index) const;
    String getEventsJson() const;

private:
    EventLog buffer[MAX_EVENTS];
    int head;
    int count;
};

extern EventLogger eventLogger;

#endif // EVENT_LOGGER_H
