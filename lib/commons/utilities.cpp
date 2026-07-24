#include "utilities.h"

#include <time.h>

#define LOADING_DOT_INTERVAL_MS 250UL

constexpr int NTP_SYNC_MIN_YEAR = 2024;

const char* formatDateTime() {
  static char buf[20];
  time_t now = time(nullptr);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  return buf;
}

bool isNtpSynced() {
  return isNtpSynced(time(nullptr));
}

bool isNtpSynced(time_t now) {
  struct tm timeInfo;
  gmtime_r(&now, &timeInfo);
  return (timeInfo.tm_year + 1900) >= NTP_SYNC_MIN_YEAR;
}

void copyFromFlash(char* dest, size_t destSize, PGM_P src) {
  strncpy_P(dest, src, destSize);
  dest[destSize - 1] = '\0';
}

void urlEncode(char* dest, size_t destSize, const char* src) {
  static const char hexDigits[] = "0123456789ABCDEF";
  size_t j = 0;
  for (size_t i = 0; src[i] && j + 1 < destSize; ++i) {
    char c = src[i];
    if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' ||
        c == '~' || c == ',') {
      dest[j++] = c;
      continue;
    }
    if (j + 3 >= destSize)
      break;
    dest[j++] = '%';
    dest[j++] = hexDigits[(c >> 4) & 0x0F];
    dest[j++] = hexDigits[c & 0x0F];
  }
  dest[j] = '\0';
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
  Serial.printf_P(connected ? PSTR(".connected\n") : PSTR(".disconnected\n"));
  return connected;
}

bool waitNtp(uint32_t timeoutMs) {
  Serial.println(F("Waiting for NTP sync..."));
  const uint32_t startMs = millis();
  while (!isNtpSynced() && (millis() - startMs) <= timeoutMs) {
    Serial.print(F("."));
    delay(LOADING_DOT_INTERVAL_MS);
  }
  const bool synced = isNtpSynced();
  Serial.printf_P(synced ? PSTR(".synced (%s)\n") : PSTR(".not synced\n"),
                  formatDateTime());
  return synced;
}
