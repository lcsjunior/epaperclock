# epaperclock

ESP8266 firmware (PlatformIO/Arduino) for a 1.54" e-paper clock. Shows a 24h
digital clock (NTP, configured timezone) and the current weather — condition
icon + temperature in °C — from the OpenWeatherMap *Current Weather* API.

> **Status:** the display hasn't arrived, so the e-paper is not implemented yet.
> `src/main.cpp` is a bare `LED_BUILTIN` blink; the e-paper deps (`GxEPD2`,
> `Adafruit GFX`) are out of `platformio.ini`. Layout and pinout below are for
> bring-up.

## Display layout

<img src="assets/display-mockup.svg" alt="1.54 inch e-paper clock mockup: weather on top, date in the middle, large 24h time below" width="280">

Weather on top (condition icon + temperature in °C), the date in the middle,
and the large 24h time (HH:MM) below.

## Pinout (e-paper ↔ D1 mini Pro)

Board: LOLIN `d1_mini_pro` (v2). This is the official GxEPD2 mapping for a
Waveshare SPI e-Paper on a Wemos D1 mini. Only `CLK`/`DIN` are fixed by hardware
SPI; `CS`/`DC`/`RST`/`BUSY` are set in the GxEPD2 constructor
(`epd(CS, DC, RST, BUSY)`).

| E-paper | D1 mini Pro | GPIO |
|---|---|---|
| BUSY | D2 | GPIO4  |
| RST  | D4 | GPIO2  |
| DC   | D3 | GPIO0  |
| CS   | D8 | GPIO15 |
| CLK (SCK)  | D5 | GPIO14 |
| DIN (MOSI) | D7 | GPIO13 |
| GND  | G   | GND |
| 3.3V | 3V3 | 3.3V |

Display: Waveshare 1.54" e-Paper (V2) — a 200×200 monochrome (black/white) SPI
panel driven by the `SSD1681` controller, with partial-refresh support.

References:

- [D1 mini Pro pinout](https://www.wemos.cc/en/latest/d1/d1_mini_pro.html)
- [Waveshare 1.54" e-Paper — ESP32/8266 wiring](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module_Manual#ESP32.2F8266)

## Weather

OpenWeather *Current Weather Data* (not One Call 3.0):

```
https://api.openweathermap.org/data/2.5/weather?q=Juiz%20de%20Fora,BR&units=metric&appid=<API_KEY>
```

Response fields used: `main.temp`, `weather[0].icon` / `weather[0].id` (icon
mapping), `weather[0].description`.

## TODO

- [ ] Re-add `GxEPD2` + `Adafruit GFX` and bring up the 1.54" panel.
- [ ] WiFi provisioning via a `wifi_setup` module (WiFiManager captive portal),
      based on the reference project. Portal custom config: OpenWeather API key,
      weather city/country (default `Juiz de Fora,BR`), and clock timezone
      (default `America/Sao_Paulo`).
- [ ] NTP time sync (default `America/Sao_Paulo`).
- [ ] OpenWeatherMap fetch for Juiz de Fora (icon + °C), with a fallback when
      the data can't be retrieved (no WiFi / API error) — the clock must keep
      running, only the weather area degrades (stale/placeholder).
- [ ] Partial-refresh rendering of the time, date (weekday + dd/mm) and weather;
      periodic full refresh.
- [ ] Add JST battery (LiPo) for portable power.
- [ ] Add an RTC to keep time without WiFi — RobotDyn DS1307 shield for D1 mini
      (I2C).
- [ ] Measure local temperature with an SHT30 sensor (I2C).
- [ ] Deep sleep to save battery (wake on timer, update, sleep; needs
      `D0`/`GPIO16` wired to `RST`).
