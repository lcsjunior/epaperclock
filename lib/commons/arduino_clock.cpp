#include "arduino_clock.h"

#include <time.h>

constexpr int NTP_SYNC_MIN_YEAR = 2025;

bool isClockSynced() {
  return isClockSynced(time(nullptr));
}

bool isClockSynced(time_t now) {
  struct tm timeInfo;
  gmtime_r(&now, &timeInfo);
  if ((timeInfo.tm_year + 1900) < NTP_SYNC_MIN_YEAR) {
    Serial.printf_P(PSTR("Clock not synced (year=%d)\n"),
                    timeInfo.tm_year + 1900);
    return false;
  }
  return true;
}

const char* formatDateTime() {
  static char buf[20];
  time_t now = time(nullptr);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  return buf;
}
