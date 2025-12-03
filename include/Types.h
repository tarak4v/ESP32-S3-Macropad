/**
 * @file Types.h
 * @brief Common type definitions and enums
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// ============================================================================
// ENUMERATIONS
// ============================================================================

/**
 * @brief Application state machine states
 */
enum class AppState : uint8_t {
  SPLASH,       ///< Boot splash screen
  NORMAL,       ///< Normal operation mode
  MENU,         ///< Menu navigation
  SETTINGS,     ///< Settings screen
  POST_TEST     ///< Power-on self test
};

/**
 * @brief Menu items
 */
enum class MenuItem : uint8_t {
  PROFILE_SELECT,
  SETTINGS,
  ABOUT,
  FACTORY_RESET,
  BACK
};

/**
 * @brief Settings items
 */
enum class SettingItem : uint8_t {
  BRIGHTNESS,
  DEBOUNCE,
  LONG_PRESS,
  KEYMAP_SELECT,
  SAVE,
  BACK
};

/**
 * @brief Input event types
 */
enum class InputEvent : uint8_t {
  NONE,
  KEY_PRESS,
  ENCODER_CW,
  ENCODER_CCW,
  ENCODER_SHORT_PRESS,
  ENCODER_LONG_PRESS
};

/**
 * @brief LED status colors
 */
enum class LEDStatus : uint8_t {
  LED_OFF,
  LED_RED,
  LED_GREEN,
  LED_BLUE,
  LED_YELLOW,
  LED_CYAN,
  LED_MAGENTA,
  LED_WHITE
};

/**
 * @brief HID modifier keys (bitmask)
 */
enum HIDModifier : uint8_t {
  MOD_NONE    = 0x00,
  MOD_LCTRL   = 0x01,
  MOD_LSHIFT  = 0x02,
  MOD_LALT    = 0x04,
  MOD_LGUI    = 0x08,
  MOD_RCTRL   = 0x10,
  MOD_RSHIFT  = 0x20,
  MOD_RALT    = 0x40,
  MOD_RGUI    = 0x80
};

// ============================================================================
// STRUCTURES
// ============================================================================

/**
 * @brief Input event data
 */
struct Input {
  InputEvent type;
  union {
    char keyChar;         ///< For KEY_PRESS events
    int8_t encoderDelta;  ///< For encoder rotation events
  };
  
  Input() : type(InputEvent::NONE), keyChar(0) {}
};

/**
 * @brief Key action definition
 */
struct KeyAction {
  uint8_t modifiers;    ///< HID modifier bitmask
  uint8_t keycode;      ///< HID keycode
  uint16_t consumer;    ///< Consumer control code (0 if not used)
  
  KeyAction() : modifiers(0), keycode(0), consumer(0) {}
  KeyAction(uint8_t mod, uint8_t key) 
    : modifiers(mod), keycode(key), consumer(0) {}
  KeyAction(uint16_t cons) 
    : modifiers(0), keycode(0), consumer(cons) {}
};

/**
 * @brief Profile definition
 */
struct Profile {
  char name[16];                  ///< Profile name
  KeyAction keys[16];             ///< Key mappings (0-9, A-D, *, #)
  uint32_t color;                 ///< Profile color (RGB888)
  
  Profile() : color(0xFFFFFF) {
    name[0] = '\0';
  }
};

/**
 * @brief System configuration
 */
struct SystemConfig {
  uint8_t currentProfile;
  uint8_t brightness;
  uint8_t debounceTime;
  uint16_t longPressTime;
  char keymapName[32];
  
  SystemConfig() 
    : currentProfile(0)
    , brightness(128)
    , debounceTime(10)
    , longPressTime(1000) {
    keymapName[0] = '\0';
  }
};

/**
 * @brief POST test results
 */
struct POSTResults {
  bool i2c_ok;
  bool display_ok;
  bool keypad_ok;
  bool encoder_ok;
  bool encoder_btn_ok;
  bool usb_ok;
  bool spiffs_ok;
  
  POSTResults() 
    : i2c_ok(false)
    , display_ok(false)
    , keypad_ok(false)
    , encoder_ok(false)
    , encoder_btn_ok(false)
    , usb_ok(false)
    , spiffs_ok(false) {}
    
  bool allPassed() const {
    return i2c_ok && display_ok && keypad_ok && 
           encoder_ok && encoder_btn_ok && usb_ok && spiffs_ok;
  }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * @brief Get key index from character (0-15)
 */
inline int getKeyIndex(char key) {
  const char* keys = "123A456B789C*0#D";
  for (int i = 0; i < 16; i++) {
    if (keys[i] == key) return i;
  }
  return -1;
}

/**
 * @brief Convert RGB to 24-bit color
 */
inline uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

/**
 * @brief Extract RGB components from 24-bit color
 */
inline void getRGB(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b) {
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = color & 0xFF;
}

#endif // TYPES_H
