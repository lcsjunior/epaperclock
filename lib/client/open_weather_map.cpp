#include "open_weather_map.h"

#include <ArduinoJson.h>
#include <utilities.h>

#ifndef OWM_HTTP_TIMEOUT_MS
#define OWM_HTTP_TIMEOUT_MS 8000UL
#endif

#ifndef OWM_REFRESH_INTERVAL_MS
#define OWM_REFRESH_INTERVAL_MS 600000UL
#endif

static const char OWM_URL[] PROGMEM =
    "http://api.openweathermap.org/data/2.5/"
    "weather?units=metric&q=%s&appid=%s";

OpenWeatherMap OWM;

void OpenWeatherMap::begin(const char* apiKey, const char* location) {
  apiKey_ = apiKey;
  location_ = location;
  lastRefreshMs_ = millis() - OWM_REFRESH_INTERVAL_MS;
}

void OpenWeatherMap::refresh() {
  if (!hasConfig())
    return;
  if (millis() - lastRefreshMs_ < OWM_REFRESH_INTERVAL_MS)
    return;
  http_.begin(wifiClient_, formatUrl());
  http_.setTimeout(OWM_HTTP_TIMEOUT_MS);
  processResponse();
  http_.end();
  lastRefreshMs_ = millis();
}

float OpenWeatherMap::temperature() const {
  return temperature_;
}

const char* OpenWeatherMap::description() const {
  return description_;
}

bool OpenWeatherMap::hasConfig() {
  if (!apiKey_ || !apiKey_[0]) {
    Serial.println(F("OWM apiKey not configured"));
    return false;
  }
  if (!location_ || !location_[0]) {
    Serial.println(F("OWM location not configured"));
    return false;
  }
  return true;
}

const char* OpenWeatherMap::formatUrl() {
  static char url[196];
  static char encodedLocation[100];
  urlEncode(encodedLocation, sizeof(encodedLocation), location_);
  snprintf_P(url, sizeof(url), OWM_URL, encodedLocation, apiKey_);
  return url;
}

void OpenWeatherMap::processResponse() {
  int statusCode = http_.GET();
  if (statusCode != HTTP_CODE_OK) {
    Serial.printf_P(PSTR("OWM GET failed, code=%d\n"), statusCode);
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, http_.getStream());
  if (error) {
    Serial.printf_P(PSTR("OWM parse error: %s\n"), error.c_str());
    return;
  }

  setTemperature(doc["main"]["temp"].as<float>());
  setDescription(doc["weather"][0]["description"]);

  Serial.printf_P(PSTR("OWM %.2f°C %s\n"), temperature_, description_);
}

void OpenWeatherMap::setTemperature(float value) {
  temperature_ = value;
}

void OpenWeatherMap::setDescription(const char* value) {
  strlcpy(description_, value, sizeof(description_));
}
