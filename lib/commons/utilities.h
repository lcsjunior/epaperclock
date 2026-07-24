#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <time.h>

void copyFromFlash(char* dest, size_t destSize, PGM_P src);
void urlEncode(char* dest, size_t destSize, const char* src);
const char* getApName();
const char* formatDateTime();
bool isNtpSynced();
bool isNtpSynced(time_t now);
bool waitWifi(uint32_t timeoutMs);
bool waitNtp(uint32_t timeoutMs);

#endif  // UTILITIES_H
