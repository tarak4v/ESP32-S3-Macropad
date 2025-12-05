/**
 * @file roboeyes_mode.h
 * @brief RoboEyes animated display mode
 * @author tarak4v
 * @date 2025-12-05
 */

#ifndef ROBOEYES_MODE_H
#define ROBOEYES_MODE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"

// Forward declaration to avoid including RoboEyes in header
template<typename T> class RoboEyes;

/**
 * @brief RoboEyes mode manager
 * 
 * Provides interactive robot eyes animation on the OLED display.
 * Keypad controls for expressions, moods, and animations.
 * 
 * Keypad Controls:
 * 1 = Happy    2 = Tired    3 = Angry    A = Default
 * 4 = Blink    5 = Laugh    6 = Confused B = Close/Open
 * 7 = Look N   8 = Look E   9 = Look S   C = Look W
 * * = Curious  0 = Idle     # = Sweat    D = Cyclops
 */
class RoboEyesMode {
public:
  /**
   * @brief Constructor
   * @param display Reference to OLED display
   */
  RoboEyesMode(Adafruit_SSD1306& display);
  
  /**
   * @brief Initialize RoboEyes
   */
  void begin();
  
  /**
   * @brief Update animation (call in loop)
   */
  void update();
  
  /**
   * @brief Handle keypad input
   * @param key Pressed key character
   */
  void handleKey(char key);
  
  /**
   * @brief Reset to default state
   */
  void reset();
  
  /**
   * @brief Destructor
   */
  ~RoboEyesMode();
  
  /**
   * @brief Check if mode is active
   */
  bool isActive() const { return _active; }

private:
  Adafruit_SSD1306& _display;
  RoboEyes<Adafruit_SSD1306>* _eyes;  // Use pointer for pimpl idiom
  bool _active;
  uint8_t _currentMood;
  bool _eyesOpen;
  bool _curiousMode;
  bool _cyclopsMode;
  bool _idleMode;
  bool _sweatMode;
  
  // Helper functions
  void setMood(char key);
  void playAnimation(char key);
  void setPosition(char key);
  void toggleMode(char key);
};

#endif // ROBOEYES_MODE_H
