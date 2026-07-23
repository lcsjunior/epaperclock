#include "arduino_clock.h"

#include <time.h>

const char* formatLocalDateTime() {
  static char buf[20];
  time_t now = time(nullptr);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  return buf;
}
