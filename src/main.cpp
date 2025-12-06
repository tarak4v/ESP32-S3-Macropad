/**
 * @file main.cpp
 * @brief Minimal welcome screen with Wi-Fi/Bluetooth status icons.
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <NimBLEDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace {

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr int8_t OLED_RESET_PIN = -1;
constexpr uint8_t OLED_I2C_ADDRESS = 0x3C;
constexpr int OLED_SDA_PIN = 14;
constexpr int OLED_SCL_PIN = 13;
constexpr uint32_t DISPLAY_REFRESH_MS = 500;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

volatile bool gBleConnected = false;
unsigned long gLastRender = 0;

class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer*) override {
    gBleConnected = true;
  }

  void onDisconnect(NimBLEServer*) override {
    gBleConnected = false;
    NimBLEDevice::startAdvertising();
  }
};

ServerCallbacks gBleCallbacks;

void drawWifiIcon(int16_t x, int16_t y) {
  const int16_t cx = x + 7;
  const int16_t cy = y + 5;

  for (int radius = 6; radius >= 2; radius -= 2) {
    display.drawCircleHelper(cx, cy, radius, 0x1, SSD1306_WHITE);
    display.drawCircleHelper(cx, cy, radius, 0x2, SSD1306_WHITE);
  }

  display.drawLine(cx - 3, cy + 3, cx + 3, cy + 3, SSD1306_WHITE);
  display.drawLine(cx - 2, cy + 4, cx + 2, cy + 4, SSD1306_WHITE);
  display.fillCircle(cx, cy + 6, 1, SSD1306_WHITE);
}

void drawBluetoothIcon(int16_t x, int16_t y) {
  const int16_t cx = x + 3;
  const int16_t top = y;

  display.drawLine(cx, top, cx, top + 15, SSD1306_WHITE);
  display.drawLine(cx, top, cx + 5, top + 4, SSD1306_WHITE);
  display.drawLine(cx + 5, top + 4, cx, top + 8, SSD1306_WHITE);
  display.drawLine(cx, top + 8, cx + 5, top + 12, SSD1306_WHITE);
  display.drawLine(cx + 5, top + 12, cx, top + 16, SSD1306_WHITE);
}

void renderScreen() {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(16, 8);
  display.print(F("Tarak's HID"));

  const bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  if (wifiConnected) {
    drawWifiIcon(SCREEN_WIDTH - 16, 0);
  }

  if (gBleConnected) {
    drawBluetoothIcon(SCREEN_WIDTH - 16, 20);
  }

  display.setTextSize(1);
  display.setCursor(0, 44);
  display.print(F("IP: "));

  if (wifiConnected) {
    display.print(WiFi.localIP());
  } else {
    display.print(F("--.--.--.--"));
  }

  display.display();
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}

void setupBle() {
  NimBLEDevice::init("ESP32S3-MacroPad");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  NimBLEServer* server = NimBLEDevice::createServer();
  server->setCallbacks(&gBleCallbacks);

  NimBLEService* service = server->createService("180F");
  service->start();

  NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
  advertising->addServiceUUID(service->getUUID());
  advertising->start();
}

} // namespace

void setup() {
  Serial.begin(115200);
  delay(100);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println("Display initialization failed");
    while (true) {
      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(16, 24);
  display.print(F("Tarak's HID"));
  display.display();

  setupWifi();
  setupBle();
}

void loop() {
  const unsigned long now = millis();
  if (now - gLastRender >= DISPLAY_REFRESH_MS) {
    gLastRender = now;
    renderScreen();
  }

  delay(10);
}
