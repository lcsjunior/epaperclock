#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

void copyFromFlash(char* dest, size_t destSize, PGM_P src);
const char* getApName();
bool waitWifi(uint32_t timeoutMs);

#endif  // UTILITIES_H
