#include <Arduino.h>

static constexpr uint8_t kLedPin = 21;
static constexpr uint8_t kBrightness = 16;
static constexpr uint32_t kBlinkHalfPeriodMs = 500;

static bool ledOn = false;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("[esp-homekit] verification firmware booted");
    Serial.printf("[esp-homekit] built " __DATE__ " " __TIME__ "\n");
}

void loop() {
    if (ledOn) {
        neopixelWrite(kLedPin, kBrightness, 0, 0);
    } else {
        neopixelWrite(kLedPin, 0, 0, 0);
    }
    ledOn = !ledOn;
    delay(kBlinkHalfPeriodMs);
}
