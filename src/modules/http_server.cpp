#include "http_server.h"
#include "wifi_setup.h"

#include <open_weather_map.h>
#include <utilities.h>

static const char APPLICATION_JSON[] PROGMEM = "application/json";

void initHttpServer() {
  wifiManager.server->on("/health", HTTP_GET, []() {
    char buf[192];
    snprintf_P(
        buf, sizeof(buf),
        PSTR("{\"status\":\"UP\",\"datetime\":\"%s\",\"temperature\":%.2f,"
             "\"weather\":\"%s\"}"),
        formatDateTime(), OWM.temperature(), OWM.description());
    wifiManager.server->send(200, FPSTR(APPLICATION_JSON), buf);
  });
}
