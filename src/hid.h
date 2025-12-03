/**
 * @file hid.h
 * @brief USB HID keyboard and media control wrapper
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef HID_H
#define HID_H

#include <Arduino.h>
#include <USB.h>
#include <USBHID.h>
#include "Config.h"
#include "Types.h"

// Consumer control codes (media keys)
#define HID_CONSUMER_MUTE           0x00E2
#define HID_CONSUMER_VOLUME_UP      0x00E9
#define HID_CONSUMER_VOLUME_DOWN    0x00EA
#define HID_CONSUMER_PLAY_PAUSE     0x00CD
#define HID_CONSUMER_NEXT_TRACK     0x00B5
#define HID_CONSUMER_PREV_TRACK     0x00B6
#define HID_CONSUMER_STOP           0x00B7

// Common HID keyboard codes
#define HID_KEY_A                   0x04
#define HID_KEY_C                   0x06
#define HID_KEY_V                   0x19
#define HID_KEY_X                   0x1B
#define HID_KEY_Z                   0x1D
#define HID_KEY_Y                   0x1C
#define HID_KEY_F                   0x09
#define HID_KEY_S                   0x16
#define HID_KEY_ENTER               0x28
#define HID_KEY_ESC                 0x29
#define HID_KEY_TAB                 0x2B
#define HID_KEY_SPACE               0x2C

/**
 * @brief HID interface for keyboard and media control
 * 
 * Provides simple wrapper around USB HID functionality for
 * sending keyboard keys and consumer control (media) commands.
 */
class HIDInterface {
public:
  /**
   * @brief Constructor
   */
  HIDInterface();
  
  /**
   * @brief Initialize USB HID
   * @return true if initialization successful
   */
  bool begin();
  
  /**
   * @brief Check if HID is connected
   * @return true if USB connection active
   */
  bool isConnected();
  
  /**
   * @brief Send keyboard key with modifiers
   * @param modifiers Modifier keys (see HIDModifier enum)
   * @param keycode HID keycode
   */
  void sendKey(uint8_t modifiers, uint8_t keycode);
  
  /**
   * @brief Send keyboard key without modifiers
   * @param keycode HID keycode
   */
  void sendKey(uint8_t keycode);
  
  /**
   * @brief Send consumer control code (media keys)
   * @param code Consumer control code
   */
  void sendConsumerKey(uint16_t code);
  
  /**
   * @brief Send key action (from profile)
   * @param action KeyAction structure
   */
  void sendAction(const KeyAction& action);
  
  /**
   * @brief Release all keys
   */
  void releaseAll();
  
  /**
   * @brief Get connection status string
   * @return Status string for debugging
   */
  String getStatus();

private:
  USBHID _hid;
  bool _initialized;
  bool _connected;
  unsigned long _lastStatusCheck;
  
  // HID report descriptor
  static const uint8_t _reportDescriptor[];
  static const size_t _reportDescriptorSize;
  
  // Send keyboard report
  void sendReport(uint8_t modifiers, uint8_t keycode);
};

#endif // HID_H
