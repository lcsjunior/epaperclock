#include "rtc_clock.h"

#include <arduino_clock.h>
#include <coredecls.h>
#include <time.h>

Ds1307Rtc RTC;

void Ds1307Rtc::begin() {
  present_ = rtc_.begin();
  if (!present_) {
    Serial.println(F("RTC not detected"));
    return;
  }
  settimeofday_cb(onTimeSynced);
}

void Ds1307Rtc::read(struct tm* out) {
  DateTime now = rtc_.now();
  out->tm_year = now.year() - 1900;
  out->tm_mon = now.month() - 1;
  out->tm_mday = now.day();
  out->tm_hour = now.hour();
  out->tm_min = now.minute();
  out->tm_sec = now.second();
  out->tm_isdst = 0;
}

const char* Ds1307Rtc::formatDateTime() {
  static char buf[20];
  struct tm t;
  read(&t);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
  return buf;
}

void Ds1307Rtc::write(const struct tm& value) {
  if (!present_) {
    Serial.println(F("RTC not detected, skipping write"));
    return;
  }
  rtc_.adjust(DateTime(value.tm_year + 1900, value.tm_mon + 1, value.tm_mday,
                       value.tm_hour, value.tm_min, value.tm_sec));
}

void Ds1307Rtc::onTimeSynced() {
  if (!isClockSynced()) {
    return;
  }
  time_t now = time(nullptr);
  struct tm local;
  localtime_r(&now, &local);
  RTC.write(local);
}
