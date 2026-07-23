#include <Arduino.h>
#include <config.h>

#include "modules/http_server.h"
#include "modules/ota_setup.h"
#include "modules/wifi_setup.h"

#define SERIAL_BAUD_RATE 115200

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  AppConfig.mount();
  AppConfig.load();

  initWifi();
  initOta();
  initHttpServer();

  Serial.printf_P(PSTR("weather location: %s\n"), AppConfig.weatherLocation());
  Serial.printf_P(PSTR("timezone: %s\n"), AppConfig.timezone());
}

void loop() {
  wifiManager.process();
  ArduinoOTA.handle();
}
