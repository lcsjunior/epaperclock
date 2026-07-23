#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Config {
 public:
  bool mount();
  bool load();
  bool save();

  const char* otaPass() const;
  const char* owmApiKey() const;
  const char* weatherLocation() const;
  const char* timezone() const;

  void setOtaPass(const char* value);
  void setOwmApiKey(const char* value);
  void setWeatherLocation(const char* value);
  void setTimezone(const char* value);

 private:
  char otaPass_[16];
  char owmApiKey_[40];
  char weatherLocation_[64];
  char timezone_[48];

  void applyDefaults();
  void convertFromJson(const JsonDocument& doc);
  void convertToJson(JsonDocument& doc) const;
};

extern Config AppConfig;

#endif  // CONFIG_H
