#include <Arduino.h>
#include <HomeSpan.h>

static constexpr uint8_t kLedPin = 21;
static constexpr uint32_t kStartupDelayMs = 3000;

void setup() {
    Serial.begin(115200);
    delay(kStartupDelayMs);
    Serial.println();
    Serial.println("[esp-homekit] firmware booted, starting HomeSpan");
    Serial.printf("[esp-homekit] built " __DATE__ " " __TIME__ "\n");

    homeSpan.setPairingCode("11122333");
    homeSpan.enableAutoStartAP();
    homeSpan.begin(Category::Switches, "esp-homekit switch", "esp-homekit");

    new SpanAccessory();
    new Service::AccessoryInformation();
    new Characteristic::Identify();

    new Service::Switch();

    rgbLedWrite(kLedPin, 0, 0, 0);
}

void loop() {
    homeSpan.poll();
}
