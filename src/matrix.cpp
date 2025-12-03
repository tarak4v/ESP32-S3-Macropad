/**
 * @file matrix.cpp
 * @brief Keypad matrix scanning implementation
 * @author tarak4v
 * @date 2025-12-03
 */

#include "matrix.h"

MatrixScanner::MatrixScanner() 
  : _keypad(nullptr)
  , _lastKey(0)
  , _lastKeyTime(0)
  , _initialized(false) {
}

bool MatrixScanner::begin() {
  DEBUG_PRINTLN("MatrixScanner: Initializing...");
  
  // Create keypad instance with configuration from Config.h
  _keypad = new Keypad(
    makeKeymap((char(*)[KEYPAD_COLS])KEYPAD_KEYS),
    (byte*)KEYPAD_ROW_PINS,
    (byte*)KEYPAD_COL_PINS,
    KEYPAD_ROWS,
    KEYPAD_COLS
  );
  
  if (!_keypad) {
    DEBUG_PRINTLN("MatrixScanner: Failed to allocate keypad");
    return false;
  }
  
  // Set debounce time
  _keypad->setDebounceTime(DEBOUNCE_TIME_MS);
  
  _initialized = true;
  DEBUG_PRINTLN("MatrixScanner: Initialized successfully");
  return true;
}

void MatrixScanner::update() {
  if (!_initialized || !_keypad) {
    return;
  }
  
  // Update internal keypad state
  // The Keypad library handles scanning internally
}

bool MatrixScanner::getKey(char& key) {
  if (!_initialized || !_keypad) {
    return false;
  }
  
  char pressed = _keypad->getKey();
  
  if (pressed) {
    // Apply additional software debounce
    unsigned long now = millis();
    if (now - _lastKeyTime > DEBOUNCE_TIME_MS) {
      key = pressed;
      _lastKey = pressed;
      _lastKeyTime = now;
      
      #if DEBUG_INPUT
      DEBUG_PRINTF("MatrixScanner: Key pressed: %c\n", key);
      #endif
      
      return true;
    }
  }
  
  return false;
}

bool MatrixScanner::isKeyPressed(char key) {
  if (!_initialized || !_keypad) {
    return false;
  }
  
  KeyState state = _keypad->getState();
  return (state == PRESSED || state == HOLD) && (_keypad->getKey() == key);
}

String MatrixScanner::getState() {
  if (!_initialized || !_keypad) {
    return "NOT_INITIALIZED";
  }
  
  KeyState state = _keypad->getState();
  switch (state) {
    case IDLE: return "IDLE";
    case PRESSED: return "PRESSED";
    case HOLD: return "HOLD";
    case RELEASED: return "RELEASED";
    default: return "UNKNOWN";
  }
}
