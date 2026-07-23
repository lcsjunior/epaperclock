# Project Conventions

Target is the **ESP8266** (`d1_mini_pro`) only. Structure and naming mirror the
reference project <https://github.com/lcsjunior/aquacontrol32-4r>, but it is an
ESP32 — its logging and string handling do not transfer.

When a rule here conflicts with a hardware constraint (pinout, timing, memory),
the hardware wins.

## Memory

- No `malloc()`/`new` at runtime; allocate in global constructors only.
- Never use Arduino `String` — `char[]` + `snprintf`.
- Fixed-size buffers, sized from the protocol/payload.
- Stack is ~4 KB: large buffers must be global or `static`, never locals.
- When diagnosing, fragmentation matters more than free heap:
  `ESP.getHeapFragmentation()` (>50% is trouble), `ESP.getMaxFreeBlockSize()`.

## Flash strings

String literals live in RAM by default. Keep constant strings in flash:

```cpp
Serial.println(F("inline flash string"));
Serial.printf_P(PSTR("value: %s\n"), name);
static const char banner[] PROGMEM = "stored in flash";
Serial.println(FPSTR(banner));
strcpy_P(buf, PSTR("copied from flash"));
```

- `print`/`println` take `F()`/`FPSTR()` — the type selects the flash path.
- Every plain-`const char*` API (`printf`, `strcpy`, `strcmp`, `snprintf`, …)
  needs the `_P` variant *and* a `PSTR`/`PROGMEM` source. Mixing them silently
  reads garbage.
- The `_P` suffix follows the **source** pointer, never the destination: a
  setter taking an arbitrary `const char*` keeps the plain variant.
- There is no `strlcpy_P` — use `copyFromFlash()` from `lib/commons`.

## Constants

- Default to `constexpr` (`constexpr const char*`, `constexpr int`, …).
- A string read through the `_P` API must be a `PROGMEM` **array** — `PROGMEM`
  on a pointer moves the pointer, not the string.
- `#define` (`UPPER_SNAKE_CASE`) only for pins and for timeouts/intervals, the
  latter with a `_MS` suffix and `UL` literal.

## Timing

- Never `delay()` in `loop()`; use elapsed-time checks against `millis()`.
- The single core is cooperatively scheduled — keep `loop()` iterations short so
  the WiFi stack runs and the watchdog stays fed.

## ISR / callbacks

- Handlers set flags or toggle outputs, nothing else. No `delay()`, `Serial`,
  or sensor reads.
- Mark them `IRAM_ATTR`.
- `Wire`, `SPI` and `Serial` only from the main loop.

## Hardware access

- Reach sensors and actuators through their abstraction layer, never the driver
  directly; assume throttling lives inside it.
- A direct call outside that layer needs a comment saying why.

## Naming

Arduino convention, which overrides any naming recommended in skills:

- `PascalCase` — classes and types.
- `camelCase` — methods, functions, variables, attributes.
- `UPPER_SNAKE_CASE` — macros and pins.
- Interfaces take the plain name (`TemperatureSensor`, no `I` prefix); when it
  collides with a concrete class, that class gets the `Impl` suffix.
- Global singletons are `PascalCase` like the Arduino core libs (`Serial`,
  `Wire`): `extern DisplayManager Display;` in the header, defined at the top of
  the `.cpp`, right after the includes.

## Style

- Code, identifiers and log strings in **English**.
- No comments — names must reveal intent (`heaterRelay`, not `obj`).
- Class sections in order: `public:` (constructor first), `protected:`,
  `private:` (attributes before methods).
- Prefer early return and the ternary over nested `if/else`.
- Log via `Serial.print`/`println`/`printf` — the ESP8266 core has no `log_*`.

## Dependencies

Pinned by version in `platformio.ini`. Do not add, remove or update one without
explicit approval.

## Documentation

Keep `README.md` (the product) and `CLAUDE.md` (how to work in the repo) current
in the same change set, without overlap between them.
