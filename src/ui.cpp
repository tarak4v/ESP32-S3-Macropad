/**
 * @file ui.cpp
 * @brief OLED display UI implementation
 * @author tarak4v
 * @date 2025-12-03
 */

#include "ui.h"

UIManager::UIManager() 
  : _display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET)
  , _initialized(false)
  , _lastUpdate(0)
  , _brightness(BRIGHTNESS_DEFAULT) {
}

bool UIManager::begin() {
  DEBUG_PRINTLN("UIManager: Initializing...");
  
  // Initialize I2C
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  
  // Initialize display
  if (!_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    DEBUG_PRINTLN("UIManager: SSD1306 allocation failed");
    return false;
  }
  
  _display.clearDisplay();
  _display.setTextColor(SSD1306_WHITE);
  _display.display();
  
  _initialized = true;
  DEBUG_PRINTLN("UIManager: Initialized successfully");
  
  return true;
}

void UIManager::setBrightness(uint8_t brightness) {
  _brightness = brightness;
  if (_initialized) {
    _display.ssd1306_command(SSD1306_SETCONTRAST);
    _display.ssd1306_command(brightness);
  }
}

void UIManager::update() {
  if (!_initialized) {
    return;
  }
  
  unsigned long now = millis();
  if (now - _lastUpdate < DISPLAY_REFRESH_MS) {
    return;  // Rate limit updates
  }
  
  _display.display();
  _lastUpdate = now;
  
  #if DEBUG_DISPLAY
  DEBUG_PRINTLN("UIManager: Display updated");
  #endif
}

void UIManager::drawSplash(const char* version) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  _display.setTextSize(2);
  _display.setCursor(10, 10);
  _display.println(F("MACROPAD"));
  
  _display.setTextSize(1);
  _display.setCursor(30, 35);
  _display.println(F("ESP32-S3"));
  
  _display.setCursor(20, 50);
  _display.print(F("v"));
  _display.println(version);
  
  _display.display();
}

void UIManager::drawNormal(const char* profileName, bool hidConnected) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  
  // Header
  drawHeader(profileName);
  
  // Connection status
  _display.setTextSize(1);
  _display.setCursor(0, 20);
  _display.print(F("HID: "));
  _display.println(hidConnected ? F("CONNECTED") : F("DISCONNECTED"));
  
  // Ready indicator
  _display.setTextSize(2);
  _display.setCursor(10, 40);
  _display.println(F("READY"));
  
  _display.display();
}

void UIManager::drawProfileMenu(const char** profiles, uint8_t profileCount, uint8_t selectedIndex) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  drawHeader("Select Profile");
  
  // Draw profile list (show up to 4 profiles)
  int startIdx = max(0, (int)selectedIndex - 1);
  int y = 16;
  
  for (int i = startIdx; i < min((int)profileCount, startIdx + 4); i++) {
    _display.setTextSize(1);
    _display.setCursor(4, y);
    
    if (i == selectedIndex) {
      _display.print(F("> "));
      _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      _display.print(profiles[i]);
      _display.setTextColor(SSD1306_WHITE);
    } else {
      _display.print(F("  "));
      _display.print(profiles[i]);
    }
    
    y += 12;
  }
  
  drawFooter("Turn: Select | Press: Confirm");
  _display.display();
}

void UIManager::drawSettings(SettingItem selectedItem, uint8_t brightness, 
                             uint8_t debounce, uint16_t longPress) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  drawHeader("Settings");
  
  int y = 16;
  const char* items[] = {
    "Brightness", "Debounce", "Long Press", "Keymap", "Save", "Back"
  };
  
  for (int i = 0; i < 6; i++) {
    _display.setTextSize(1);
    _display.setCursor(4, y);
    
    if ((SettingItem)i == selectedItem) {
      _display.print(F("> "));
    } else {
      _display.print(F("  "));
    }
    
    _display.print(items[i]);
    
    // Show values for adjustable settings
    if (i == 0) {
      _display.print(F(": "));
      _display.print(brightness);
    } else if (i == 1) {
      _display.print(F(": "));
      _display.print(debounce);
      _display.print(F("ms"));
    } else if (i == 2) {
      _display.print(F(": "));
      _display.print(longPress);
      _display.print(F("ms"));
    }
    
    y += 10;
  }
  
  _display.display();
}

void UIManager::drawPOST(const POSTResults& results) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  _display.setTextSize(1);
  
  drawHeader("POST Results");
  
  int y = 16;
  _display.setCursor(0, y); y += 10;
  _display.print(F("I2C:     ")); _display.println(results.i2c_ok ? F("OK") : F("FAIL"));
  
  _display.setCursor(0, y); y += 10;
  _display.print(F("Display: ")); _display.println(results.display_ok ? F("OK") : F("FAIL"));
  
  _display.setCursor(0, y); y += 10;
  _display.print(F("Keypad:  ")); _display.println(results.keypad_ok ? F("OK") : F("FAIL"));
  
  _display.setCursor(0, y); y += 10;
  _display.print(F("Encoder: ")); _display.println(results.encoder_ok ? F("OK") : F("FAIL"));
  
  _display.setCursor(0, y); y += 10;
  _display.print(F("Enc.Btn: ")); _display.println(results.encoder_btn_ok ? F("OK") : F("FAIL"));
  
  drawFooter(results.allPassed() ? "READY" : "CHECK FAIL");
  _display.display();
}

void UIManager::drawPOSTTest(const char* title, const char* status, bool passed) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  drawHeader("POST - Hardware Test");
  
  _display.setTextSize(1);
  _display.setCursor(0, 20);
  _display.println(title);
  
  _display.setCursor(0, 36);
  _display.println(status);
  
  _display.setTextSize(2);
  _display.setCursor(20, 48);
  _display.print(passed ? F("[ OK ]") : F("[FAIL]"));
  
  _display.display();
}

void UIManager::drawHIDStatus(bool connected) {
  if (!_initialized) return;
  
  _display.clearDisplay();
  drawHeader("POST Complete");
  
  _display.setTextSize(2);
  _display.setCursor(20, 24);
  _display.println(F("HID"));
  
  _display.setCursor(4, 42);
  _display.println(connected ? F("CONNECTED") : F("WAITING..."));
  
  _display.setTextSize(1);
  drawFooter(connected ? "Ready to use!" : "Connect USB/BT");
  
  _display.display();
}

void UIManager::clear() {
  if (!_initialized) return;
  _display.clearDisplay();
  _display.display();
}

// Private helper functions

void UIManager::drawHeader(const char* title) {
  _display.setTextSize(1);
  _display.setCursor(0, 0);
  _display.println(title);
  _display.drawLine(0, 10, OLED_WIDTH - 1, 10, SSD1306_WHITE);
}

void UIManager::drawFooter(const char* text) {
  _display.drawLine(0, OLED_HEIGHT - 12, OLED_WIDTH - 1, OLED_HEIGHT - 12, SSD1306_WHITE);
  _display.setTextSize(1);
  _display.setCursor(0, OLED_HEIGHT - 8);
  _display.print(text);
}

void UIManager::drawProgressBar(int x, int y, int width, int height, float progress) {
  _display.drawRect(x, y, width, height, SSD1306_WHITE);
  int fillWidth = (int)((width - 2) * progress);
  _display.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}

void UIManager::drawCenteredText(const char* text, int y, uint8_t textSize) {
  _display.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  _display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  _display.setCursor((OLED_WIDTH - w) / 2, y);
  _display.print(text);
}
