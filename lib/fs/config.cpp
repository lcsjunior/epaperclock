#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <utilities.h>

constexpr const char* CONFIG_PATH = "/config.json";

static const char DEFAULT_WEATHER_LOCATION[] PROGMEM = "Juiz de Fora,BR";
static const char DEFAULT_TIMEZONE[] PROGMEM = "America/Sao_Paulo";

Config AppConfig;

bool Config::mount() {
  if (LittleFS.begin()) {
    return true;
  }
  Serial.println(F("Failed to mount LittleFS, formatting"));
  if (LittleFS.format() && LittleFS.begin()) {
    return true;
  }
  Serial.println(F("Failed to format LittleFS"));
  return false;
}

bool Config::load() {
  applyDefaults();

  File file = LittleFS.open(CONFIG_PATH, "r");
  if (!file) {
    Serial.printf_P(PSTR("Failed to open %s\n"), CONFIG_PATH);
    return false;
  }

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, file);
  size_t bytesRead = file.size();
  file.close();

  if (err) {
    Serial.printf_P(PSTR("Parse error: %s\n"), err.c_str());
    return false;
  }

  Serial.printf_P(PSTR("Loaded %u bytes from %s\n"), bytesRead, CONFIG_PATH);
  convertFromJson(doc);
  return true;
}

bool Config::save() {
  JsonDocument doc;
  convertToJson(doc);

  File file = LittleFS.open(CONFIG_PATH, "w");
  if (!file) {
    Serial.printf_P(PSTR("Failed to open %s for writing\n"), CONFIG_PATH);
    return false;
  }

  size_t bytesWritten = serializeJson(doc, file);
  file.close();
  if (bytesWritten == 0) {
    Serial.printf_P(PSTR("Failed to write to %s\n"), CONFIG_PATH);
    return false;
  }

  Serial.printf_P(PSTR("Saved %u bytes to %s\n"), bytesWritten, CONFIG_PATH);
  return true;
}

const char* Config::otaPass() const {
  return otaPass_;
}

const char* Config::owmApiKey() const {
  return owmApiKey_;
}

const char* Config::weatherLocation() const {
  return weatherLocation_;
}

const char* Config::timezone() const {
  return timezone_;
}

void Config::setOtaPass(const char* value) {
  strlcpy(otaPass_, value, sizeof(otaPass_));
}

void Config::setOwmApiKey(const char* value) {
  strlcpy(owmApiKey_, value, sizeof(owmApiKey_));
}

void Config::setWeatherLocation(const char* value) {
  strlcpy(weatherLocation_, value, sizeof(weatherLocation_));
}

void Config::setTimezone(const char* value) {
  strlcpy(timezone_, value, sizeof(timezone_));
}

void Config::applyDefaults() {
  setOtaPass(OTA_PASSWORD);
  owmApiKey_[0] = '\0';
  copyFromFlash(weatherLocation_, sizeof(weatherLocation_),
                DEFAULT_WEATHER_LOCATION);
  copyFromFlash(timezone_, sizeof(timezone_), DEFAULT_TIMEZONE);
}

void Config::convertFromJson(const JsonDocument& doc) {
  applyDefaults();

  const char* otaPass = doc["ota_pass"];
  const char* owmApiKey = doc["owm_api_key"];
  const char* weatherLocation = doc["weather_location"];
  const char* timezone = doc["timezone"];

  if (otaPass) {
    setOtaPass(otaPass);
  }
  if (owmApiKey) {
    setOwmApiKey(owmApiKey);
  }
  if (weatherLocation) {
    setWeatherLocation(weatherLocation);
  }
  if (timezone) {
    setTimezone(timezone);
  }
}

void Config::convertToJson(JsonDocument& doc) const {
  doc["ota_pass"] = otaPass_;
  doc["owm_api_key"] = owmApiKey_;
  doc["weather_location"] = weatherLocation_;
  doc["timezone"] = timezone_;
}
