#include "utilities.h"

#define LOADING_DOT_INTERVAL_MS 250UL

void copyFromFlash(char* dest, size_t destSize, PGM_P src) {
  strncpy_P(dest, src, destSize);
  dest[destSize - 1] = '\0';
}

const char* getApName() {
  static char name[32];
  snprintf_P(name, sizeof(name), PSTR("ESPsoftAP-%lu"),
             (unsigned long)ESP.getChipId());
  return name;
}

bool waitWifi(uint32_t timeoutMs) {
  Serial.println(F("Waiting for WiFi connection..."));
  const uint32_t startMs = millis();
  while (!WiFi.isConnected() && (millis() - startMs) <= timeoutMs) {
    Serial.print(F("."));
    delay(LOADING_DOT_INTERVAL_MS);
  }
  const bool connected = WiFi.isConnected();
  Serial.println(connected ? F("connected") : F("disconnected"));
  return connected;
}
