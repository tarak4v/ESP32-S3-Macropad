/**
 * @file hid.cpp
 * @brief USB HID implementation
 * @author tarak4v
 * @date 2025-12-03
 */

#include "hid.h"

// HID Report Descriptor for keyboard and consumer control
const uint8_t HIDInterface::_reportDescriptor[] = {
  // Keyboard Report
  0x05, 0x01,        // Usage Page (Generic Desktop)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Keyboard)
  0x19, 0xE0,        //   Usage Minimum (Left Control)
  0x29, 0xE7,        //   Usage Maximum (Right GUI)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x81, 0x02,        //   Input (Data, Variable, Absolute)
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x08,        //   Report Size (8)
  0x81, 0x01,        //   Input (Constant) - Reserved byte
  0x95, 0x06,        //   Report Count (6)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x65,        //   Logical Maximum (101)
  0x05, 0x07,        //   Usage Page (Keyboard)
  0x19, 0x00,        //   Usage Minimum (0)
  0x29, 0x65,        //   Usage Maximum (101)
  0x81, 0x00,        //   Input (Data, Array)
  0xC0,              // End Collection
  
  // Consumer Control Report
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x19, 0x00,        //   Usage Minimum (0)
  0x2A, 0x3C, 0x02,  //   Usage Maximum (572)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0x3C, 0x02,  //   Logical Maximum (572)
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x10,        //   Report Size (16)
  0x81, 0x00,        //   Input (Data, Array)
  0xC0               // End Collection
};

const size_t HIDInterface::_reportDescriptorSize = sizeof(_reportDescriptor);

HIDInterface::HIDInterface() 
  : _initialized(false)
  , _connected(false)
  , _lastStatusCheck(0) {
}

bool HIDInterface::begin() {
  DEBUG_PRINTLN("HIDInterface: Initializing...");
  
  // Initialize USB HID
  _hid.begin();
  
  // Start USB
  USB.begin();
  
  delay(500);  // Allow USB to enumerate
  
  _initialized = true;
  _connected = true;  // Assume connected after initialization
  
  DEBUG_PRINTF("HIDInterface: Initialized (Connected: %s)\n", 
               _connected ? "YES" : "NO");
  
  return true;
}

bool HIDInterface::isConnected() {
  if (!_initialized) {
    return false;
  }
  
  // Check if USB is mounted/ready
  // Note: ESP32-S3 Arduino framework doesn't have USB.connected()
  // We'll assume connected if initialized
  _connected = true;
  
  return _connected;
}

void HIDInterface::sendKey(uint8_t modifiers, uint8_t keycode) {
  if (!_initialized || !isConnected()) {
    #if DEBUG_HID
    DEBUG_PRINTLN("HIDInterface: Cannot send key - not connected");
    #endif
    return;
  }
  
  sendReport(modifiers, keycode);
  delay(10);  // Small delay for key press
  sendReport(0, 0);  // Release
  
  #if DEBUG_HID
  DEBUG_PRINTF("HIDInterface: Sent key 0x%02X with modifiers 0x%02X\n", 
               keycode, modifiers);
  #endif
}

void HIDInterface::sendKey(uint8_t keycode) {
  sendKey(MOD_NONE, keycode);
}

void HIDInterface::sendConsumerKey(uint16_t code) {
  if (!_initialized || !isConnected()) {
    #if DEBUG_HID
    DEBUG_PRINTLN("HIDInterface: Cannot send consumer key - not connected");
    #endif
    return;
  }
  
  // Consumer control report
  uint8_t report[2] = {
    (uint8_t)(code & 0xFF),  // Low byte
    (uint8_t)(code >> 8)     // High byte
  };
  
  _hid.SendReport(2, report, sizeof(report));
  delay(10);
  
  // Release
  uint8_t releaseReport[2] = {0x00, 0x00};
  _hid.SendReport(2, releaseReport, sizeof(releaseReport));
  
  #if DEBUG_HID
  DEBUG_PRINTF("HIDInterface: Sent consumer key 0x%04X\n", code);
  #endif
}

void HIDInterface::sendAction(const KeyAction& action) {
  if (action.consumer != 0) {
    // Consumer control action (media key)
    sendConsumerKey(action.consumer);
  } else {
    // Keyboard action
    sendKey(action.modifiers, action.keycode);
  }
}

void HIDInterface::releaseAll() {
  if (!_initialized) {
    return;
  }
  
  sendReport(0, 0);
}

String HIDInterface::getStatus() {
  if (!_initialized) {
    return "NOT_INITIALIZED";
  }
  
  return _connected ? "CONNECTED" : "DISCONNECTED";
}

void HIDInterface::sendReport(uint8_t modifiers, uint8_t keycode) {
  // Keyboard report structure
  uint8_t report[8] = {
    modifiers,  // Modifier byte
    0x00,       // Reserved
    keycode,    // Key 1
    0x00,       // Key 2
    0x00,       // Key 3
    0x00,       // Key 4
    0x00,       // Key 5
    0x00        // Key 6
  };
  
  _hid.SendReport(1, report, sizeof(report));
}
