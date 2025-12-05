/**
 * @file Config.h
 * @brief Configuration constants and pin definitions
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

// OLED Display (I2C)
#define OLED_SDA_PIN      21
#define OLED_SCL_PIN      22
#define OLED_ADDRESS      0x3C
#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_RESET        -1

// Keypad Matrix (4x4)
#define KEYPAD_ROWS       4
#define KEYPAD_COLS       4
const byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {4, 5, 6, 7};
const byte KEYPAD_COL_PINS[KEYPAD_COLS] = {15, 16, 17, 18};

// Keypad Layout
const char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Rotary Encoder
#define ENCODER_PIN_A     8
#define ENCODER_PIN_B     9
#define ENCODER_BTN_PIN   10

// Status LEDs (RGB LED on ESP32-S3-DevKitC-1)
#define LED_RED_PIN       47
#define LED_GREEN_PIN     21
#define LED_BLUE_PIN      48

// ============================================================================
// TIMING CONFIGURATION
// ============================================================================

#define DEBOUNCE_TIME_MS          10    // Keypad debounce
#define ENCODER_DEBOUNCE_MS       5     // Encoder debounce
#define LONG_PRESS_THRESHOLD_MS   1000  // Long press detection
#define DISPLAY_REFRESH_MS        33    // 30 FPS
#define INPUT_POLL_MS             5     // Input polling rate
#define STATUS_UPDATE_MS          500   // LED status update rate

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================

#define MAX_PROFILES              8     // Maximum number of profiles
#define MAX_PROFILE_NAME_LEN      16    // Max profile name length
#define KEYMAP_PATH               "/keymaps/default.json"
#define CONFIG_NAMESPACE          "macropad"

// Display brightness levels
#define BRIGHTNESS_MIN            0
#define BRIGHTNESS_MAX            255
#define BRIGHTNESS_DEFAULT        128

// Volume control
#define VOLUME_SENSITIVITY        1     // Encoder steps per volume change

// ============================================================================
// DEFAULT CONFIGURATION
// ============================================================================

struct DefaultConfig {
  static constexpr uint8_t profile = 0;
  static constexpr uint8_t brightness = BRIGHTNESS_DEFAULT;
  static constexpr uint8_t debounce = DEBOUNCE_TIME_MS;
  static constexpr uint16_t longPress = LONG_PRESS_THRESHOLD_MS;
  static constexpr const char* keymap = "default";
};

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

#define DEBUG_SERIAL              true
#define DEBUG_BAUD_RATE           115200
#define DEBUG_INPUT               false  // Log input events
#define DEBUG_DISPLAY             false  // Log display updates
#define DEBUG_HID                 false  // Log HID reports
#define DEBUG_CONFIG              true   // Log config operations

#if DEBUG_SERIAL
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

// ============================================================================
// OTA CONFIGURATION
// ============================================================================

#define OTA_ENABLED               false
#define OTA_HOSTNAME              "Tarak-HID-Host"
#define OTA_PORT                  3232

// WiFi credentials (change these for your network)
#define WIFI_SSID                 "Tarak 2.4_EXT"
#define WIFI_PASSWORD             "1Tarak3385"

// ============================================================================
// VERSION INFORMATION
// ============================================================================

#define FIRMWARE_VERSION          "1.0.0"
#define FIRMWARE_DATE             "2025-12-05"
#define FIRMWARE_NAME             "Tarak-HID"
#define FIRMWARE_AUTHOR           "tarak4v"

#endif // CONFIG_H
