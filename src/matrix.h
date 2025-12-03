/**
 * @file matrix.h
 * @brief Keypad matrix scanning with debounce
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <Keypad.h>
#include "Config.h"
#include "Types.h"

/**
 * @brief Matrix keypad scanner with debounce
 * 
 * Handles 4x4 matrix keypad scanning with hardware debounce.
 * Provides simple interface for polling key states.
 */
class MatrixScanner {
public:
  /**
   * @brief Constructor
   */
  MatrixScanner();
  
  /**
   * @brief Initialize the keypad hardware
   * @return true if initialization successful
   */
  bool begin();
  
  /**
   * @brief Update keypad state (call frequently)
   * 
   * This should be called in the main loop to update the
   * internal keypad state and handle debouncing.
   */
  void update();
  
  /**
   * @brief Get pressed key if available
   * @param key Output parameter for the key character
   * @return true if a key was pressed, false otherwise
   */
  bool getKey(char& key);
  
  /**
   * @brief Check if a specific key is currently pressed
   * @param key Key character to check
   * @return true if key is pressed
   */
  bool isKeyPressed(char key);
  
  /**
   * @brief Get raw keypad state for debugging
   * @return Keypad state as string
   */
  String getState();

private:
  Keypad* _keypad;
  char _lastKey;
  unsigned long _lastKeyTime;
  bool _initialized;
};

#endif // MATRIX_H
