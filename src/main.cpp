#include <Arduino.h>
#include <HomeSpan.h>

static constexpr uint8_t kLedPin = 21;
static constexpr uint8_t kBrightness = 16;
static constexpr uint32_t kBlinkHalfPeriodMs = 500;
static constexpr uint32_t kStartupDelayMs = 3000;

static bool ledOn = false;

static void setLed(uint8_t red, uint8_t green, uint8_t blue) {
    rgbLedWrite(kLedPin, red, green, blue);
}

void setup() {
    Serial.begin(115200);
    delay(kStartupDelayMs);
    Serial.println();
    Serial.println("[esp-homekit] verification firmware booted");
    Serial.printf("[esp-homekit] built " __DATE__ " " __TIME__ "\n");

    homeSpan.setPairingCode("11122333");
    homeSpan.begin(Category::Switches, "esp-homekit switch", "esp-homekit");

    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();

    new Service::Switch();
}

void loop() {
    homeSpan.poll();

    if (ledOn) {
        setLed(kBrightness, 0, 0);
    } else {
        setLed(0, 0, 0);
    }
    ledOn = !ledOn;
    delay(kBlinkHalfPeriodMs);
}
