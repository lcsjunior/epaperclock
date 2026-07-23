#include "ota_setup.h"

#include <Arduino.h>
#include <config.h>

void initOta() {
  ArduinoOTA.setHostname(DEVICE_HOSTNAME);
  ArduinoOTA.setPassword(AppConfig.otaPass());

  ArduinoOTA.onStart([]() { Serial.println(F("OTA start")); });
  ArduinoOTA.onEnd([]() { Serial.println(F("OTA end")); });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf_P(PSTR("OTA error %u\n"), error);
  });

  ArduinoOTA.begin();
}
