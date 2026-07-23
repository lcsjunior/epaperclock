#include "http_server.h"

#include <arduino_clock.h>
#include <rtc_clock.h>

#include "wifi_setup.h"

static const char APPLICATION_JSON[] PROGMEM = "application/json";

void initHttpServer() {
  wifiManager.server->on("/health", HTTP_GET, []() {
    char buf[128];
    snprintf_P(buf, sizeof(buf),
               PSTR("{\"status\":\"UP\",\"datetime\":{\"ntp\":\"%s\",\"rtc\":\"%s\"}}"),
               formatDateTime(), RTC.formatDateTime());
    wifiManager.server->send(200, FPSTR(APPLICATION_JSON), buf);
  });
}
