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

static bool deviceState = false;
static bool deviceConnected = false;
static BLEServer* pServer = nullptr;

static void setLed(uint8_t red, uint8_t green, uint8_t blue) {
    neopixelWrite(kLedPin, green, red, blue);
}

class CommandCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) override {
        String value = pCharacteristic->getValue();
        if (value.length() != 1) {
            return;
        }
        deviceState = (value[0] != 0x00);
        setLed(deviceState ? kBrightness : 0, 0, 0);
    }
};

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* server) override {
        deviceConnected = true;
        Serial.println("[esp-homekit] client connected");
    }

    void onDisconnect(BLEServer* server) override {
        deviceConnected = false;
        Serial.println("[esp-homekit] client disconnected; restarting advertising");
        server->getAdvertising()->start();
        Serial.println("[esp-homekit] advertising restarted");
    }
};

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

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());
    BLEService* pService = pServer->createService(kServiceUUID);
    BLECharacteristic* pCommand = pService->createCharacteristic(
        kCharacteristicUUID,
        BLECharacteristic::PROPERTY_WRITE);
    pCommand->setCallbacks(new CommandCallbacks());
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
    setLed(deviceState ? kBrightness : 0, 0, 0);
    delay(kBlinkHalfPeriodMs);
}
