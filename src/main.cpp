#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

static const char* kDeviceName = "esp-homekit-ble";
static const char* kServiceUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
static const char* kCharacteristicUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

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
    if (!BLEDevice::init(kDeviceName)) {
        Serial.println("[esp-homekit] BLE initialization failed!");
        return;
    }

    BLEServer* pServer = BLEDevice::createServer();
    pServer->advertiseOnDisconnect(true);
    BLEService* pService = pServer->createService(kServiceUUID);
    BLECharacteristic* pCharacteristic = pService->createCharacteristic(
        kCharacteristicUUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setValue("esp-homekit");
    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(kServiceUUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);
    BLEDevice::startAdvertising();
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
