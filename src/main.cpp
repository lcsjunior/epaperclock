#include <Arduino.h>

#define SERIAL_BAUD_RATE 115200

#define BLINK_INTERVAL_MS 2000UL

static unsigned long lastBlinkMs = 0;
static bool ledOn = false;

void toggleLed() {
  ledOn = !ledOn;
  digitalWrite(LED_BUILTIN, ledOn ? LOW : HIGH);
  Serial.printf_P(PSTR("led %s\n"), ledOn ? "on" : "off");
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("boot"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledOn ? LOW : HIGH);
}

void loop() {
  unsigned long nowMs = millis();

  if (nowMs - lastBlinkMs >= BLINK_INTERVAL_MS) {
    lastBlinkMs = nowMs;
    toggleLed();
  }
}
