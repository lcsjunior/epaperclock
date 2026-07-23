# Project Conventions

## Reference project

- Structure, naming, and firmware patterns follow the reference project
  <https://github.com/lcsjunior/aquacontrol32-4r>. When in doubt, mirror how it
  is done there.

## Embedded C++ (firmware baseline)

Mandatory rules for every firmware file (ESP32 and ESP8266). Consult before
implementing or reviewing. Where the two chips differ, the platform is called
out explicitly.

### Memory

- No `malloc()` or `new` at runtime — heap fragmentation is fatal in embedded;
  allocate only in global constructors.
- Use fixed-size buffers: `char buf[32]`, `char msg[255]` — size according to
  protocol/payload constraints.
- Never use the Arduino `String` class — use `char[]` + `snprintf`. On ESP8266
  its repeated (re)allocations are a top cause of heap fragmentation and silent
  crashes.
- **ESP8266:** stack is only ~4 KB — keep large objects/buffers out of the stack
  (make them global or `static`), never place big arrays as locals.
- **ESP8266:** heap fragmentation (not just total free heap) is what kills the
  device; when diagnosing, check `ESP.getHeapFragmentation()` (>50% is
  problematic) and `ESP.getMaxFreeBlockSize()`.

### Flash strings (string literals)

- **ESP8266:** `const char*` literals live in **RAM by default** — prefer flash
  storage for constant strings to save RAM. Use `F("...")` inline,
  `PSTR("...")` with `printf_P`, and `PROGMEM` + `FPSTR()` for globals:

  ```cpp
  Serial.println(F("inline flash string"));
  Serial.printf_P(PSTR("value: %s\n"), name);
  static const char banner[] PROGMEM = "stored in flash";
  Serial.println(FPSTR(banner));
  ```

- **ESP8266 — pick the tool by how the reader learns it is flash.** Storage
  (`PSTR`/`F`/`PROGMEM`) and reading are two halves:
  - `print`/`println` have an overload for `const __FlashStringHelper*`, so
    `F("...")` (or `FPSTR(global)`) is enough — the type selects the flash path;
    there is no `println_P`.
  - Plain-`const char*` APIs (`printf`, `strcpy`, `strcat`, `strcmp`, `strlen`,
    `memcpy`, `snprintf`, …) **cannot tell** a flash pointer from a RAM one, so
    the string must be `PSTR("...")`/`PROGMEM` **and** you must call the `_P`
    variant that reads from flash: `printf_P`, `strcpy_P`, `strcat_P`,
    `strcmp_P`, `strlen_P`, `memcpy_P`, `snprintf_P`, etc.

  ```cpp
  char buf[32];
  strcpy_P(buf, PSTR("copied from flash"));
  if (strcmp_P(cmd, PSTR("reset")) == 0) { /* ... */ }
  snprintf_P(buf, sizeof(buf), PSTR("id=%d"), id);
  ```

  `F(x)` is just `PSTR(x)` cast to `__FlashStringHelper*`; a bare `PSTR`/flash
  pointer passed to a non-`_P` function reads garbage, and a `F()` value does not
  compile where a `const char*` is expected.

- **ESP32:** these macros are no-ops/aliases (`const` data is already in
  flash-mapped rodata) — use plain string literals, `snprintf`, `strcpy`,
  `Serial.print`, etc. directly; do not add `F()`/`PSTR()`/`_P` noise.

### ISR / callbacks

- Timer and interrupt callbacks must be minimal: toggle outputs or set flags only.
- Never call `delay()`, `Serial`, or sensor reads inside an ISR or timer callback.
- Do not invoke `Wire`, `SPI`, or `Serial` outside the main loop.
- **ESP8266:** mark interrupt handlers `IRAM_ATTR` (formerly `ICACHE_RAM_ATTR`)
  so they run from RAM and don't fault while flash is busy.

### Sensor access

- Access sensors only through their abstraction layer — never call driver methods
  directly from application code.
- Assume throttling and timing are handled inside the abstraction; do not
  replicate that logic externally.

### Output / actuator control

- Drive actuators only through the designated control layer (e.g., a thermostat
  or state machine handler).
- Direct actuator calls outside that layer must include a comment explaining the
  reason.

### Non-blocking timers

- Use non-blocking timer patterns (elapsed-time checks against `millis()`) for
  all periodic tasks.
- Never use `delay()` in `loop()` — it stalls WiFi, OTA, MQTT, and all
  cooperative tasks.
- **ESP8266:** the single core is cooperatively scheduled — a long-running
  `loop()` starves the WiFi stack; yield often (short `loop()` iterations, or
  `yield()`/`delay(0)`) so the RTOS background tasks run and the watchdog stays
  fed.

## Language

- Code, identifiers, comments, and log strings: **English**.

## Naming

Follows the Arduino convention, which overrides the naming recommended in skills:

- `PascalCase` — classes/types (e.g.: `Relay`, `Thermostat`, `DSTempSensor`).
- `camelCase` — methods, functions, variables, and attributes (e.g.: `turnOn`, `getCTemp`).
- `UPPER_SNAKE_CASE` — macros and pin definitions (e.g.: `K1_PIN`, `DS18B20_PIN_1`).

### Interfaces (pure abstract classes)

- Use idiomatic C++ style: direct name **without `I` prefix** (e.g.: `TemperatureSensor`, `ThermostatState`).
- When the interface name collides with a concrete class, the **concrete class receives the `Impl` suffix** (e.g.: `ThermostatImpl`).

### Global singletons

Following the style of Arduino core libs (`Serial`, `Wire`, `SPI`, `EEPROM`), unique global
instances use **PascalCase** (not camelCase).

Declaration/definition pattern:

- In the **header** (`.h`): declare with `extern` after the class declaration
  (e.g.: `extern DisplayManager Display;`).
- In the **source** (`.cpp`): define the instance at the **top of the file**,
  right after the `#include`s and any file-level macros, before method
  implementations (e.g.: `DisplayManager Display;`).

In all other cases, follow the conventions recommended in the skills.

## Class structure

Section order in every class declaration:

1. `public:` — constructor/destructor, then public methods.
2. `private:` — attributes, then private methods.

`protected:` follows the same logic when needed (between `public:` and `private:`).

## Dependencies

- Libraries are pinned by version in `platformio.ini`.
- Do not add, remove, or update dependencies without explicit approval.

## Code comments

- Do not add comments to code. Well-named identifiers are sufficient.
- Method, function, variable, and **object** names must be **descriptive**: reveal intent and behavior without requiring additional explanation.
- Object names must be suggestive of their role — `temperatureSensor` or `heaterRelay` instead of generic `obj` or `v`.

## Logging

- **ESP32:** use the ESP-IDF macros (`log_i`, `log_w`, `log_e`, `log_d`) for
  logging; never `Serial.print`/`Serial.println` directly.
- **ESP8266** (`d1_mini_pro`, this project): the core has no native ESP-IDF
  `log_*` macros, so log directly via
  `Serial.print`/`Serial.println`/`Serial.printf`.

## Constants

- Constants in general should be declared as `constexpr const char*` (or equivalent type, e.g.: `constexpr int`), to guarantee compile-time evaluation and pointer/value immutability.
- Exceptions allowed as `#define` (`UPPER_SNAKE_CASE`):
  - **Pins** (e.g.: `#define K1_PIN 21`).
  - **Timeouts and time intervals**: `_MS` suffix and `UL` literal (e.g.: `#define MQTT_CONN_TIMEOUT_MS 5000UL`).

## Control flow

- Prefer early return and the ternary operator over nested `if/else` blocks.

## Hardware as source of truth

When a C++ guideline conflicts with a hardware constraint (pinout,
timing, ESP32/ESP8266 memory), the hardware constraint takes precedence.

## Project documentation

- `README.md`, `CLAUDE.md`, and `AGENTS.md` must always be kept up to date.
- Whenever a change affects architecture, conventions, hardware wiring, dependencies,
  or any information already documented in these files, update them in the same
  change set.
