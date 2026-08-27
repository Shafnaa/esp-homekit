#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

static const char* kDeviceName = "esp-homekit-ble";

static constexpr uint8_t kLedPin = 21;
static constexpr uint8_t kBrightness = 16;
static constexpr uint32_t kBlinkHalfPeriodMs = 500;
static constexpr uint32_t kStartupDelayMs = 3000;

static bool ledOn = false;

static void setLed(uint8_t red, uint8_t green, uint8_t blue) {
    neopixelWrite(kLedPin, green, red, blue);
}

void setup() {
    Serial.begin(115200);
    delay(kStartupDelayMs);
    Serial.println();
    Serial.println("[esp-homekit] verification firmware booted");
    Serial.printf("[esp-homekit] built " __DATE__ " " __TIME__ "\n");

    Serial.printf("[esp-homekit] initializing BLE as '%s'\n", kDeviceName);
    BLEDevice::init(kDeviceName);
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
    Serial.println("[esp-homekit] BLE advertising started");
}

void loop() {
    if (ledOn) {
        setLed(kBrightness, 0, 0);
    } else {
        setLed(0, 0, 0);
    }
    ledOn = !ledOn;
    delay(kBlinkHalfPeriodMs);
}
