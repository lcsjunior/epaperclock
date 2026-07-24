#include <Arduino.h>
#include <config.h>
#include <open_weather_map.h>

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
  OWM.begin(AppConfig.owmApiKey(), AppConfig.owmLocation());
}

void loop() {
  wifiManager.process();
  ArduinoOTA.handle();
  OWM.refresh();
}
