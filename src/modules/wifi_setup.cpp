#include "wifi_setup.h"

#include <config.h>
#include <utilities.h>

#define WIFI_PORTAL_TIMEOUT_S 300
#define WIFI_CONNECT_TIMEOUT_MS 30000UL

WiFiManager wifiManager;

WiFiManagerParameter* wmOtaPass;
WiFiManagerParameter* wmOwmApiKey;
WiFiManagerParameter* wmWeatherLocation;
WiFiManagerParameter* wmTimezone;

static void onWifiManagerSaveParams() {
  AppConfig.setOtaPass(wmOtaPass->getValue());
  AppConfig.setOwmApiKey(wmOwmApiKey->getValue());
  AppConfig.setWeatherLocation(wmWeatherLocation->getValue());
  AppConfig.setTimezone(wmTimezone->getValue());
  AppConfig.save();
}

void initWifi() {
  // clang-format off
  wmOtaPass         = new WiFiManagerParameter("ota_pass",         "OTA Password",        AppConfig.otaPass(),         15, "type=\"password\"");
  wmOwmApiKey       = new WiFiManagerParameter("owm_api_key",      "OpenWeather API Key", AppConfig.owmApiKey(),       32, "type=\"password\"");
  wmWeatherLocation = new WiFiManagerParameter("weather_location", "Weather City,Country",AppConfig.weatherLocation(), 48);
  wmTimezone        = new WiFiManagerParameter("timezone",         "Timezone",            AppConfig.timezone(),        32);
  // clang-format on

  wifiManager.addParameter(wmOtaPass);
  wifiManager.addParameter(wmOwmApiKey);
  wifiManager.addParameter(wmWeatherLocation);
  wifiManager.addParameter(wmTimezone);

  wifiManager.setConfigPortalBlocking(false);
  wifiManager.setConfigPortalTimeout(WIFI_PORTAL_TIMEOUT_S);
  wifiManager.setHostname(DEVICE_HOSTNAME);
  wifiManager.setSaveParamsCallback(onWifiManagerSaveParams);
  wifiManager.setWiFiAutoReconnect(true);
  wifiManager.autoConnect(getApName());
  wifiManager.startWebPortal();
  waitWifi(WIFI_CONNECT_TIMEOUT_MS);
}
