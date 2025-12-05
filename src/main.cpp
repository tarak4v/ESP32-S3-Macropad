/**
 * @file main.cpp
 * @brief ESP32-S3 Macropad - POST Test with Modular Architecture
 * @author tarak4v
 * @date 2025-12-03
 * 
 * Power-On Self Test (POST) for hardware validation.
 * Tests: I2C, OLED, Keypad, Encoder, USB HID, Storage
 * 
 * Status LEDs:
 *  - GREEN: HID connected and ready
 *  - RED: HID disconnected / POST failed
 *  - BLUE: POST passed, waiting for connection
 */

#include <Arduino.h>
#include <Encoder.h>
#include "Config.h"
#include "Types.h"
#include "matrix.h"
#include "hid.h"
#include "ui.h"
#include "storage.h"
#include "ota.h"

// Module instances
MatrixScanner matrix;
HIDInterface hid;
UIManager ui;
StorageManager storage;
OTAManager ota;

// Encoder instance (not yet modularized)
Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// POST test results
POSTResults results;

// LED control helper
void setLED(bool red, bool green, bool blue) {
  digitalWrite(LED_RED_PIN, !red);     // Inverted logic for common anode
  digitalWrite(LED_GREEN_PIN, !green);
  digitalWrite(LED_BLUE_PIN, !blue);
}

// POST test functions
bool testI2C() {
  Serial.println("\n=== Testing I2C Bus ===");
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  Wire.beginTransmission(OLED_ADDRESS);
  byte error = Wire.endTransmission();
  
  results.i2c_ok = (error == 0);
  Serial.printf("I2C: %s\n", results.i2c_ok ? "OK" : "FAIL");
  return results.i2c_ok;
}

bool testDisplay() {
  Serial.println("\n=== Testing OLED Display ===");
  results.display_ok = ui.begin();
  
  if (results.display_ok) {
    ui.drawSplash(FIRMWARE_VERSION);
    delay(1000);
  }
  
  Serial.printf("Display: %s\n", results.display_ok ? "OK" : "FAIL");
  return results.display_ok;
}

bool testKeypad() {
  Serial.println("\n=== Testing Keypad ===");
  
  if (!matrix.begin()) {
    Serial.println("Matrix initialization failed!");
    results.keypad_ok = false;
    return false;
  }
  
  Serial.println("Press any key within 10 seconds...");
  ui.drawPOSTTest("Keypad Test", "Press any key", false);
  
  unsigned long startTime = millis();
  char key;
  
  while (millis() - startTime < 10000) {
    matrix.update();
    if (matrix.getKey(key)) {
      Serial.printf("Key detected: %c\n", key);
      char msg[32];
      snprintf(msg, sizeof(msg), "Key: %c", key);
      ui.drawPOSTTest("Keypad Test", msg, true);
      results.keypad_ok = true;
      delay(1000);
      return true;
    }
    delay(10);
  }
  
  Serial.println("No key pressed - TIMEOUT");
  ui.drawPOSTTest("Keypad Test", "No key detected", false);
  delay(2000);
  results.keypad_ok = false;
  return false;
}

bool testEncoder() {
  Serial.println("\n=== Testing Rotary Encoder ===");
  Serial.println("Rotate encoder within 10 seconds...");
  
  ui.drawPOSTTest("Encoder Test", "Rotate encoder", false);
  
  long startPos = encoder.read();
  unsigned long startTime = millis();
  
  while (millis() - startTime < 10000) {
    long newPos = encoder.read();
    if (abs(newPos - startPos) > 2) {
      Serial.printf("Encoder rotation detected! Delta: %ld\n", newPos - startPos);
      ui.drawPOSTTest("Encoder Test", "Rotation: OK", true);
      results.encoder_ok = true;
      delay(1000);
      return true;
    }
    delay(10);
  }
  
  Serial.println("No rotation detected - TIMEOUT");
  ui.drawPOSTTest("Encoder Test", "No rotation", false);
  delay(2000);
  results.encoder_ok = false;
  return false;
}

bool testEncoderButton() {
  Serial.println("\n=== Testing Encoder Button ===");
  Serial.println("Press encoder button within 10 seconds...");
  
  pinMode(ENCODER_BTN_PIN, INPUT_PULLUP);
  ui.drawPOSTTest("Button Test", "Press encoder", false);
  
  unsigned long startTime = millis();
  
  while (millis() - startTime < 10000) {
    if (digitalRead(ENCODER_BTN_PIN) == LOW) {
      Serial.println("Encoder button pressed!");
      ui.drawPOSTTest("Button Test", "Button: OK", true);
      results.encoder_btn_ok = true;
      delay(1000);
      return true;
    }
    delay(10);
  }
  
  Serial.println("No button press detected - TIMEOUT");
  ui.drawPOSTTest("Button Test", "No press", false);
  delay(2000);
  results.encoder_btn_ok = false;
  return false;
}

bool testUSB() {
  Serial.println("\n=== Testing USB HID ===");
  results.usb_ok = hid.begin();
  delay(500);
  Serial.printf("USB HID: %s\n", results.usb_ok ? "OK" : "FAIL");
  return results.usb_ok;
}

bool testStorage() {
  Serial.println("\n=== Testing Storage ===");
  results.spiffs_ok = storage.begin();
  Serial.printf("Storage: %s\n", results.spiffs_ok ? "OK" : "FAIL");
  return results.spiffs_ok;
}

void setup() {
  Serial.begin(DEBUG_BAUD_RATE);
  delay(1000);
  
  // Initialize LED pins
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  setLED(false, false, false);
  
  Serial.println("\n========================================");
  Serial.printf("  %s v%s\n", FIRMWARE_NAME, FIRMWARE_VERSION);
  Serial.println("  POST Test - Modular Architecture");
  Serial.println("========================================");
  
  // Run all POST tests
  testI2C();
  testDisplay();
  testUSB();
  testStorage();
  testKeypad();
  testEncoder();
  testEncoderButton();
  
  // Display final results
  Serial.println("\n========================================");
  Serial.println("POST Test Complete");
  Serial.println("========================================");
  Serial.printf("I2C:            %s\n", results.i2c_ok ? "PASS" : "FAIL");
  Serial.printf("Display:        %s\n", results.display_ok ? "PASS" : "FAIL");
  Serial.printf("USB HID:        %s\n", results.usb_ok ? "PASS" : "FAIL");
  Serial.printf("Storage:        %s\n", results.spiffs_ok ? "PASS" : "FAIL");
  Serial.printf("Keypad:         %s\n", results.keypad_ok ? "PASS" : "FAIL");
  Serial.printf("Encoder:        %s\n", results.encoder_ok ? "PASS" : "FAIL");
  Serial.printf("Encoder Button: %s\n", results.encoder_btn_ok ? "PASS" : "FAIL");
  Serial.println("========================================");
  
  // Display results on OLED
  ui.drawPOST(results);
  delay(3000);
  
  // Set LED based on results
  if (results.allPassed()) {
    Serial.println("✓ ALL TESTS PASSED");
    setLED(false, false, true);  // Blue = Ready
  } else {
    Serial.println("✗ SOME TESTS FAILED");
    setLED(true, false, false);  // Red = Error
  }
  
  delay(2000);
  
  // Initialize OTA for wireless updates
  #if OTA_ENABLED
  Serial.println("\n========================================");
  Serial.println("Initializing OTA...");
  Serial.println("========================================");
  
  if (ota.begin(WIFI_SSID, WIFI_PASSWORD, OTA_HOSTNAME, OTA_PORT)) {
    Serial.println("✓ OTA Enabled");
    Serial.printf("  Hostname: %s.local\n", OTA_HOSTNAME);
    Serial.printf("  IP: %s\n", ota.getIPAddress().c_str());
    Serial.printf("  Port: %d\n", OTA_PORT);
    Serial.println("\nReady for wireless updates!");
    Serial.println("Use: pio run -t upload --upload-port " + ota.getIPAddress());
    
    // Blink green LED 10 times to indicate successful WiFi connection
    Serial.println("\nWiFi connected - blinking green LED...");
    for (int i = 0; i < 10; i++) {
      setLED(false, true, false);  // Green ON
      delay(500);
      setLED(false, false, false); // LED OFF
      delay(500);
    }
  } else {
    Serial.println("✗ OTA initialization failed");
    Serial.println("  Continuing with USB-only mode...");
  }
  #endif
}

void loop() {
  // Handle OTA updates
  #if OTA_ENABLED
  ota.handle();
  #endif
  
  static bool lastConnectionState = false;
  static unsigned long lastStatusCheck = 0;
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  // Check HID connection status periodically
  unsigned long now = millis();
  if (now - lastStatusCheck > STATUS_UPDATE_MS) {
    bool isConnected = hid.isConnected();
    
    // Connection state changed
    if (isConnected != lastConnectionState) {
      lastConnectionState = isConnected;
      
      if (isConnected) {
        Serial.println("HID Connected");
        setLED(false, true, false);  // Green
        ui.drawHIDStatus(true);
      } else {
        Serial.println("HID Disconnected");
        setLED(true, false, false);  // Red
        ui.drawHIDStatus(false);
      }
    }
    
    // Blink blue LED if POST passed but not connected
    if (results.allPassed() && !isConnected) {
      if (now - lastBlink > 2000) {
        ledState = !ledState;
        setLED(ledState ? false : true, false, ledState ? true : false);
        lastBlink = now;
      }
    }
    
    lastStatusCheck = now;
  }
  
  // Monitor keypresses (for testing HID)
  matrix.update();
  char key;
  if (matrix.getKey(key)) {
    Serial.printf("Key: %c\n", key);
    
    // If connected, send test keystroke
    if (hid.isConnected()) {
      hid.sendKey(HID_KEY_A + (key - '1'));  // Simple mapping for demo
      Serial.println("  -> Sent via HID");
    }
  }
  
  delay(10);
}
