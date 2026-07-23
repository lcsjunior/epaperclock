#ifndef ARDUINO_CLOCK_H
#define ARDUINO_CLOCK_H

#include <Arduino.h>

bool isClockSynced();
bool isClockSynced(time_t now);
const char* formatDateTime();

#endif  // ARDUINO_CLOCK_H
