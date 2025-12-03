/**
 * @file ui.h
 * @brief OLED display UI management
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"
#include "Types.h"

/**
 * @brief OLED display UI manager
 * 
 * Handles all OLED display rendering including splash screen,
 * profile display, menu navigation, and settings screens.
 */
class UIManager {
public:
  /**
   * @brief Constructor
   */
  UIManager();
  
  /**
   * @brief Initialize display hardware
   * @return true if initialization successful
   */
  bool begin();
  
  /**
   * @brief Set display brightness
   * @param brightness 0-255
   */
  void setBrightness(uint8_t brightness);
  
  /**
   * @brief Update display content
   * 
   * Call this after changing state or data to refresh the display.
   * Actual refresh is rate-limited internally.
   */
  void update();
  
  /**
   * @brief Draw splash screen
   * @param version Firmware version string
   */
  void drawSplash(const char* version);
  
  /**
   * @brief Draw normal operation screen
   * @param profileName Current profile name
   * @param hidConnected HID connection status
   */
  void drawNormal(const char* profileName, bool hidConnected);
  
  /**
   * @brief Draw profile selection menu
   * @param profiles Array of profile names
   * @param profileCount Number of profiles
   * @param selectedIndex Currently selected profile
   */
  void drawProfileMenu(const char** profiles, uint8_t profileCount, uint8_t selectedIndex);
  
  /**
   * @brief Draw settings menu
   * @param selectedItem Currently selected setting
   * @param brightness Current brightness value
   * @param debounce Current debounce time
   * @param longPress Current long press threshold
   */
  void drawSettings(SettingItem selectedItem, uint8_t brightness, 
                    uint8_t debounce, uint16_t longPress);
  
  /**
   * @brief Draw POST test results
   * @param results POST test results structure
   */
  void drawPOST(const POSTResults& results);
  
  /**
   * @brief Draw POST test screen during testing
   * @param title Test title
   * @param status Status message
   * @param passed Test pass/fail
   */
  void drawPOSTTest(const char* title, const char* status, bool passed);
  
  /**
   * @brief Draw HID status (connected/disconnected)
   * @param connected Connection status
   */
  void drawHIDStatus(bool connected);
  
  /**
   * @brief Clear display
   */
  void clear();
  
  /**
   * @brief Get display instance (for advanced usage)
   * @return Reference to Adafruit_SSD1306 instance
   */
  Adafruit_SSD1306& getDisplay() { return _display; }

private:
  Adafruit_SSD1306 _display;
  bool _initialized;
  unsigned long _lastUpdate;
  uint8_t _brightness;
  
  // Helper drawing functions
  void drawHeader(const char* title);
  void drawFooter(const char* text);
  void drawProgressBar(int x, int y, int width, int height, float progress);
  void drawCenteredText(const char* text, int y, uint8_t textSize = 1);
};

#endif // UI_H
