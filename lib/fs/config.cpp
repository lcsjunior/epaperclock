#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <config.h>
#include <utilities.h>

constexpr const char* CONFIG_PATH = "/config.json";

static const char DEFAULT_OWM_LOCATION[] PROGMEM = "Juiz de Fora,BR";
static const char DEFAULT_TIMEZONE[] PROGMEM = "BRT3";
static const char DEFAULT_NTP_SERVER[] PROGMEM = "br.pool.ntp.org";

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

const char* Config::owmLocation() const {
  return owmLocation_;
}

const char* Config::timezone() const {
  return timezone_;
}

const char* Config::ntpServer() const {
  return ntpServer_;
}

void Config::setOtaPass(const char* value) {
  strlcpy(otaPass_, value, sizeof(otaPass_));
}

void Config::setOwmApiKey(const char* value) {
  strlcpy(owmApiKey_, value, sizeof(owmApiKey_));
}

void Config::setOwmLocation(const char* value) {
  strlcpy(owmLocation_, value, sizeof(owmLocation_));
}

void Config::setTimezone(const char* value) {
  strlcpy(timezone_, value, sizeof(timezone_));
}

void Config::setNtpServer(const char* value) {
  strlcpy(ntpServer_, value, sizeof(ntpServer_));
}

void Config::applyDefaults() {
  setOtaPass(OTA_PASSWORD);
  owmApiKey_[0] = '\0';
  copyFromFlash(owmLocation_, sizeof(owmLocation_), DEFAULT_OWM_LOCATION);
  copyFromFlash(timezone_, sizeof(timezone_), DEFAULT_TIMEZONE);
  copyFromFlash(ntpServer_, sizeof(ntpServer_), DEFAULT_NTP_SERVER);
}

void Config::convertFromJson(const JsonDocument& doc) {
  setOtaPass(doc["ota_pass"] | otaPass_);
  setOwmApiKey(doc["owm_api_key"] | owmApiKey_);
  setOwmLocation(doc["owm_location"] | owmLocation_);
  setTimezone(doc["timezone"] | timezone_);
  setNtpServer(doc["ntp_server"] | ntpServer_);
}

void Config::convertToJson(JsonDocument& doc) const {
  doc["ota_pass"] = otaPass_;
  doc["owm_api_key"] = owmApiKey_;
  doc["owm_location"] = owmLocation_;
  doc["timezone"] = timezone_;
  doc["ntp_server"] = ntpServer_;
}
