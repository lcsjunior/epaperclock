# CLAUDE.md

Guidance for Claude Code (claude.ai/code) when working in this repository.

## Overview

ESP8266 firmware (`d1_mini_pro` board) for a 1.54" e-paper clock: a 24h digital
clock (NTP, `America/Sao_Paulo`, UTC−3) plus current weather for Juiz de Fora
(condition icon + °C) from the OpenWeatherMap *Current Weather* API. Partial
refresh is prioritized. Hardware, pinout, and display layout: see `README.md`.

User-customizable settings live in the WiFiManager captive portal: the
OpenWeather API key, the weather city/country (default `Juiz de Fora,BR`), and
the clock timezone (default `America/Sao_Paulo`).

## Architecture (where things live)

- `src/main.cpp` — currently a bare `LED_BUILTIN` blink; the e-paper display
  isn't implemented yet (hardware not arrived), and the deps (`GxEPD2`,
  `Adafruit GFX`) are out of `platformio.ini`.
- `include/` — shared project headers; `lib/<LibName>/src/...` — private
  libraries auto-discovered by PlatformIO; `test/` — native test runners.
- WiFi is required for NTP and the OpenWeatherMap request; WiFi credentials, the
  OWM API key, the weather city/country, and the timezone are entered in the
  captive portal (see Overview) — not build flags.

## Build, Test, Upload

```bash
pio run                # compile
pio test -e native     # host tests
pio run --target upload # flash over USB
pio device monitor     # serial monitor @ 115200 baud
```

- **Always ask for explicit user confirmation before any upload/flash** — it
  writes to physical hardware. Upload is over USB.

## Conventions

**All code must follow `.claude/rules/code-conventions.md`** — the single source
of truth for the embedded firmware baseline, naming, class structure, logging,
flash strings, constants, and control flow. Read it before writing or reviewing.
