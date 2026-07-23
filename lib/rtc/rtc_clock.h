#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

#include <RTClib.h>

class Ds1307Rtc {
 public:
  void begin();
  void read(struct tm* out);
  const char* formatDateTime();

 private:
  RTC_DS1307 rtc_;
  bool present_ = false;

  void write(const struct tm& value);
  static void onTimeSynced();
};

extern Ds1307Rtc RTC;

#endif  // RTC_CLOCK_H
