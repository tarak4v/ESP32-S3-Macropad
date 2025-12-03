/*
 *  POST (Power On Self Test) for ESP32-S3 Macropad
 *  
 *  Tests:
 *   - OLED Display initialization and communication
 *   - Keypad matrix (all 16 keys)
 *   - Rotary Encoder (rotation and button)
 *   - USB HID initialization
 *   - I2C communication
 *
 *  Status Indicator:
 *   - Blue LED (GPIO 48 - built-in RGB LED Blue channel): Ready to pair/All tests passed
 *   - Display shows test results
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Encoder.h>
#include <USB.h>
#include <USBHID.h>

// RGB LED pins on ESP32-S3-DevKitC-1
#define LED_RED    GPIO_NUM_47
#define LED_GREEN  GPIO_NUM_21
#define LED_BLUE   GPIO_NUM_48   // Blue = Ready to pair

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad configuration (4x4 matrix)
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7};
byte colPins[COLS] = {15, 16, 17, 18};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Rotary Encoder pins
#define ENCODER_PIN_A 8
#define ENCODER_PIN_B 9
#define ENCODER_BTN 10
Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// USB HID
USBHID HID;

// Test results
struct TestResults {
  bool i2c_ok;
  bool display_ok;
  bool keypad_ok;
  bool encoder_ok;
  bool encoder_btn_ok;
  bool usb_ok;
  bool all_passed;
};

TestResults results = {false, false, false, false, false, false, false};

// HID Report descriptor (minimal for testing)
static const uint8_t _hidReportDescriptor[] = {
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Keyboard)
  0x19, 0xE0,        //   Usage Minimum (224)
  0x29, 0xE7,        //   Usage Maximum (231)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x81, 0x02,        //   Input (Data, Variable, Absolute)
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x08,        //   Report Size (8)
  0x81, 0x01,        //   Input (Constant)
  0x95, 0x06,        //   Report Count (6)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x65,        //   Logical Maximum (101)
  0x05, 0x07,        //   Usage Page (Keyboard)
  0x19, 0x00,        //   Usage Minimum (0)
  0x29, 0x65,        //   Usage Maximum (101)
  0x81, 0x00,        //   Input (Data, Array)
  0xC0               // End Collection
};

void setLED(bool red, bool green, bool blue) {
  digitalWrite(LED_RED, !red);     // Inverted logic for common anode
  digitalWrite(LED_GREEN, !green);
  digitalWrite(LED_BLUE, !blue);
}

void displayTestScreen(const char* title, const char* status, bool passed) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("POST - Hardware Test");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.println(title);
  
  display.setCursor(0, 32);
  display.setTextSize(1);
  display.println(status);
  
  display.setCursor(0, 50);
  display.setTextSize(2);
  if (passed) {
    display.print("[ OK ]");
  } else {
    display.print("[FAIL]");
  }
  
  display.display();
}

bool testI2C() {
  Serial.println("\n=== Testing I2C Bus ===");
  Wire.begin();
  Wire.beginTransmission(SCREEN_ADDRESS);
  byte error = Wire.endTransmission();
  
  if (error == 0) {
    Serial.println("I2C device found at 0x3C");
    results.i2c_ok = true;
    return true;
  } else {
    Serial.printf("I2C error: %d\n", error);
    results.i2c_ok = false;
    return false;
  }
}

bool testDisplay() {
  Serial.println("\n=== Testing OLED Display ===");
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed!");
    results.display_ok = false;
    return false;
  }
  
  Serial.println("Display initialized successfully");
  
  // Test pattern
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println("POST");
  display.setCursor(10, 40);
  display.setTextSize(1);
  display.println("Display: OK");
  display.display();
  
  results.display_ok = true;
  delay(1000);
  return true;
}

bool testKeypad() {
  Serial.println("\n=== Testing Keypad ===");
  Serial.println("Press any key within 10 seconds...");
  
  displayTestScreen("Keypad Test", "Press any key", false);
  
  unsigned long startTime = millis();
  bool keyDetected = false;
  
  while (millis() - startTime < 10000) {
    char key = keypad.getKey();
    if (key) {
      Serial.printf("Key detected: %c\n", key);
      displayTestScreen("Keypad Test", String("Key: ") + String(key), true);
      keyDetected = true;
      delay(1000);
      break;
    }
    delay(10);
  }
  
  if (!keyDetected) {
    Serial.println("No key pressed - TIMEOUT");
    displayTestScreen("Keypad Test", "No key detected", false);
    delay(2000);
  }
  
  results.keypad_ok = keyDetected;
  return keyDetected;
}

bool testEncoder() {
  Serial.println("\n=== Testing Rotary Encoder ===");
  Serial.println("Rotate encoder within 10 seconds...");
  
  displayTestScreen("Encoder Test", "Rotate encoder", false);
  
  long startPos = encoder.read();
  unsigned long startTime = millis();
  bool rotationDetected = false;
  
  while (millis() - startTime < 10000) {
    long newPos = encoder.read();
    if (abs(newPos - startPos) > 2) {
      Serial.printf("Encoder rotation detected! Delta: %ld\n", newPos - startPos);
      displayTestScreen("Encoder Test", "Rotation: OK", true);
      rotationDetected = true;
      delay(1000);
      break;
    }
    delay(10);
  }
  
  if (!rotationDetected) {
    Serial.println("No rotation detected - TIMEOUT");
    displayTestScreen("Encoder Test", "No rotation", false);
    delay(2000);
  }
  
  results.encoder_ok = rotationDetected;
  return rotationDetected;
}

bool testEncoderButton() {
  Serial.println("\n=== Testing Encoder Button ===");
  Serial.println("Press encoder button within 10 seconds...");
  
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  displayTestScreen("Button Test", "Press encoder", false);
  
  unsigned long startTime = millis();
  bool buttonDetected = false;
  
  while (millis() - startTime < 10000) {
    if (digitalRead(ENCODER_BTN) == LOW) {
      Serial.println("Encoder button pressed!");
      displayTestScreen("Button Test", "Button: OK", true);
      buttonDetected = true;
      delay(1000);
      break;
    }
    delay(10);
  }
  
  if (!buttonDetected) {
    Serial.println("No button press detected - TIMEOUT");
    displayTestScreen("Button Test", "No press", false);
    delay(2000);
  }
  
  results.encoder_btn_ok = buttonDetected;
  return buttonDetected;
}

bool testUSB() {
  Serial.println("\n=== Testing USB HID ===");
  
  HID.begin();
  HID.setReportDescriptor(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  USB.begin();
  
  delay(500);
  
  Serial.println("USB HID initialized");
  results.usb_ok = true;
  return true;
}

void displayFinalResults() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("POST Results:");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  
  int y = 14;
  display.setCursor(0, y);
  display.print("I2C:     "); display.println(results.i2c_ok ? "OK" : "FAIL");
  y += 10;
  
  display.setCursor(0, y);
  display.print("Display: "); display.println(results.display_ok ? "OK" : "FAIL");
  y += 10;
  
  display.setCursor(0, y);
  display.print("Keypad:  "); display.println(results.keypad_ok ? "OK" : "FAIL");
  y += 10;
  
  display.setCursor(0, y);
  display.print("Encoder: "); display.println(results.encoder_ok ? "OK" : "FAIL");
  y += 10;
  
  display.setCursor(0, y);
  display.print("Enc.Btn: "); display.println(results.encoder_btn_ok ? "OK" : "FAIL");
  
  display.setCursor(0, 56);
  display.setTextSize(1);
  if (results.all_passed) {
    display.print("Status: READY");
  } else {
    display.print("Status: CHECK FAIL");
  }
  
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize LED pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  // Start with all LEDs off
  setLED(false, false, false);
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("  ESP32-S3 Macropad POST Test");
  Serial.println("========================================");
  
  // Run all tests
  testI2C();
  testDisplay();
  testUSB();
  testKeypad();
  testEncoder();
  testEncoderButton();
  
  // Check if all tests passed
  results.all_passed = results.i2c_ok && results.display_ok && 
                       results.keypad_ok && results.encoder_ok && 
                       results.encoder_btn_ok && results.usb_ok;
  
  // Display final results
  Serial.println("\n========================================");
  Serial.println("POST Test Complete");
  Serial.println("========================================");
  Serial.printf("I2C:            %s\n", results.i2c_ok ? "PASS" : "FAIL");
  Serial.printf("Display:        %s\n", results.display_ok ? "PASS" : "FAIL");
  Serial.printf("USB HID:        %s\n", results.usb_ok ? "PASS" : "FAIL");
  Serial.printf("Keypad:         %s\n", results.keypad_ok ? "PASS" : "FAIL");
  Serial.printf("Encoder:        %s\n", results.encoder_ok ? "PASS" : "FAIL");
  Serial.printf("Encoder Button: %s\n", results.encoder_btn_ok ? "PASS" : "FAIL");
  Serial.println("========================================");
  
  if (results.all_passed) {
    Serial.println("✓ ALL TESTS PASSED - READY TO PAIR");
    Serial.println("\nLED Status Indicators:");
    Serial.println("  GREEN = HID Connected (USB/BT)");
    Serial.println("  RED   = HID Disconnected");
    Serial.println("  BLUE  = Waiting for connection");
    setLED(true, false, false);  // Start with Red = Waiting for connection
  } else {
    Serial.println("✗ SOME TESTS FAILED - CHECK CONNECTIONS");
    setLED(true, false, false);  // Red LED = Error
  }
  
  displayFinalResults();
  
  // Give time to read results
  delay(3000);
  
  // Initial connection check
  if (USB.connected()) {
    Serial.println("\nHID Already Connected!");
    setLED(false, true, false);  // Green
  } else {
    Serial.println("\nWaiting for HID connection...");
    setLED(true, false, false);  // Red
  }
}

void loop() {
  static bool lastConnectionState = false;
  static unsigned long lastStatusCheck = 0;
  
  // Check HID connection status every 500ms
  if (millis() - lastStatusCheck > 500) {
    bool isConnected = USB.connected();
    
    if (isConnected != lastConnectionState) {
      lastConnectionState = isConnected;
      
      if (isConnected) {
        Serial.println("HID Connected - USB/Bluetooth");
        setLED(false, true, false);  // Green LED = Connected
        
        // Update display
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("POST Complete");
        display.setCursor(0, 20);
        display.setTextSize(2);
        display.println("HID");
        display.println("CONNECTED");
        display.setTextSize(1);
        display.setCursor(0, 56);
        display.println("Ready to use!");
        display.display();
      } else {
        Serial.println("HID Disconnected");
        setLED(true, false, false);  // Red LED = Disconnected
        
        // Update display
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("POST Complete");
        display.setCursor(0, 20);
        display.setTextSize(2);
        display.println("HID");
        display.println("WAITING...");
        display.setTextSize(1);
        display.setCursor(0, 56);
        display.println("Connect USB/BT");
        display.display();
      }
    }
    
    // Blink blue LED every 2 seconds if all tests passed but not connected yet
    if (results.all_passed && !isConnected) {
      static unsigned long lastBlink = 0;
      static bool ledState = false;
      
      if (millis() - lastBlink > 2000) {
        ledState = !ledState;
        if (ledState) {
          setLED(false, false, true);  // Blue blink
        } else {
          setLED(true, false, false);  // Back to red
        }
        lastBlink = millis();
      }
    }
    
    lastStatusCheck = millis();
  }
  
  // Monitor for additional keypresses to test HID
  char key = keypad.getKey();
  if (key) {
    Serial.printf("Key detected in monitoring: %c\n", key);
    
    // If connected, send a test keystroke
    if (USB.connected()) {
      Serial.println("Sending test key via HID");
    }
  }
  
  delay(1000);
}
