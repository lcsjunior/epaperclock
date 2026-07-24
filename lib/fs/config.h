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
  const char* owmLocation() const;
  const char* timezone() const;
  const char* ntpServer() const;

  void setOtaPass(const char* value);
  void setOwmApiKey(const char* value);
  void setOwmLocation(const char* value);
  void setTimezone(const char* value);
  void setNtpServer(const char* value);

 private:
  char otaPass_[16];
  char owmApiKey_[48];
  char owmLocation_[32];
  char timezone_[16];
  char ntpServer_[32];

  void applyDefaults();
  void convertFromJson(const JsonDocument& doc);
  void convertToJson(JsonDocument& doc) const;
};

extern Config AppConfig;

#endif  // CONFIG_H
