#include "http_server.h"

#include "wifi_setup.h"

static const char APPLICATION_JSON[] PROGMEM = "application/json";
static const char HEALTH_UP[] PROGMEM = "{\"status\":\"UP\"}";

void initHttpServer() {
  wifiManager.server->on("/health", HTTP_GET, []() {
    wifiManager.server->send_P(200, APPLICATION_JSON, HEALTH_UP);
  });
}
