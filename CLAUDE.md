# CLAUDE.md

Guidance for Claude Code (claude.ai/code) when working in this repository.

## Overview

ESP8266 firmware (`d1_mini_pro`) for a 1.54" e-paper clock — see `README.md`.
The panel hasn't arrived: WiFi provisioning, settings and OTA exist, display
code does not.

## Architecture (where things live)

- `src/main.cpp` — boot sequence and `loop()`.
- `src/modules/` — application wiring: `wifi_setup` (captive portal, custom
  fields, save callback), `ota_setup` and `http_server` (routes registered on
  the WiFiManager portal server, after `initWifi()`).
- `lib/fs/` — `AppConfig`: LittleFS + ArduinoJson persistence of `/config.json`.
- `lib/commons/` — helpers shared across modules.

Anything user-configurable belongs in the portal plus `Config`, not in build
flags.

## Build, Test, Upload

```bash
pio run                # compile
pio test -e native     # host tests
pio run --target upload # flash over USB
pio device monitor     # serial monitor @ 115200 baud
```

- **Always ask for explicit user confirmation before any upload/flash** — it
  writes to physical hardware.
- USB is the default upload path; keep it that way. OTA is opt-in per run or via
  an untracked `local_settings.ini`.

## Conventions

**All code must follow `.claude/rules/code-conventions.md`** — the single source
of truth for the embedded firmware baseline, naming, class structure, logging,
flash strings, constants, and control flow. Read it before writing or reviewing.
