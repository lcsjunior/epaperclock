#include "wifi_setup.h"

#include <config.h>
#include <utilities.h>

#define WIFI_PORTAL_TIMEOUT_S 300

#ifndef WIFI_CONNECT_TIMEOUT_MS
#define WIFI_CONNECT_TIMEOUT_MS 30000UL
#endif

#ifndef NTP_SYNC_TIMEOUT_MS
#define NTP_SYNC_TIMEOUT_MS 5000UL
#endif

WiFiManager wifiManager;

WiFiManagerParameter* wmOtaPass;
WiFiManagerParameter* wmOwmApiKey;
WiFiManagerParameter* wmOwmLocation;
WiFiManagerParameter* wmTimezone;
WiFiManagerParameter* wmNtpServer;

static void onWifiManagerSaveParams() {
  AppConfig.setOtaPass(wmOtaPass->getValue());
  AppConfig.setOwmApiKey(wmOwmApiKey->getValue());
  AppConfig.setOwmLocation(wmOwmLocation->getValue());
  AppConfig.setTimezone(wmTimezone->getValue());
  AppConfig.setNtpServer(wmNtpServer->getValue());
  AppConfig.save();
}

void initWifi() {
  // clang-format off
  wmOtaPass      = new WiFiManagerParameter("ota_pass",     "OTA Password",        AppConfig.otaPass(),     15, "type=\"password\"");
  wmOwmApiKey    = new WiFiManagerParameter("owm_api_key",  "OpenWeather API Key", AppConfig.owmApiKey(),   48, "type=\"password\"");
  wmOwmLocation  = new WiFiManagerParameter("owm_location", "Weather City,Country",AppConfig.owmLocation(), 32);
  wmTimezone     = new WiFiManagerParameter("timezone",     "Timezone (POSIX TZ, ex.: BRT3)", AppConfig.timezone(), 15);
  wmNtpServer    = new WiFiManagerParameter("ntp_server",   "NTP Server",          AppConfig.ntpServer(),   32);
  // clang-format on

  wifiManager.addParameter(wmOtaPass);
  wifiManager.addParameter(wmOwmApiKey);
  wifiManager.addParameter(wmOwmLocation);
  wifiManager.addParameter(wmTimezone);
  wifiManager.addParameter(wmNtpServer);

  wifiManager.setConfigPortalBlocking(false);
  wifiManager.setConfigPortalTimeout(WIFI_PORTAL_TIMEOUT_S);
  wifiManager.setHostname(DEVICE_HOSTNAME);
  wifiManager.setSaveParamsCallback(onWifiManagerSaveParams);
  wifiManager.setWiFiAutoReconnect(true);
  wifiManager.autoConnect(getApName());
  wifiManager.startWebPortal();
  waitWifi(WIFI_CONNECT_TIMEOUT_MS);

  configTzTime(AppConfig.timezone(), AppConfig.ntpServer(), "pool.ntp.org");
  waitNtp(NTP_SYNC_TIMEOUT_MS);
}
