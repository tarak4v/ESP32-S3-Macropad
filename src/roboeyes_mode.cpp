/**
 * @file roboeyes_mode.cpp
 * @brief RoboEyes animated display mode implementation
 * @author tarak4v
 * @date 2025-12-05
 */

#include "roboeyes_mode.h"
#include <FluxGarage_RoboEyes.h>

// Mood definitions
#define MOOD_DEFAULT 0
#define MOOD_TIRED 1
#define MOOD_ANGRY 2
#define MOOD_HAPPY 3

// Position definitions
#define POS_CENTER 4

RoboEyesMode::RoboEyesMode(Adafruit_SSD1306& display)
  : _display(display)
  , _eyes(nullptr)
  , _active(false)
  , _currentMood(MOOD_DEFAULT)
  , _eyesOpen(true)
  , _curiousMode(false)
  , _cyclopsMode(false)
  , _idleMode(false)
  , _sweatMode(false) {
}

RoboEyesMode::~RoboEyesMode() {
  if (_eyes) {
    delete _eyes;
    _eyes = nullptr;
  }
}

void RoboEyesMode::begin() {
  DEBUG_PRINTLN("RoboEyesMode: Initializing...");
  
  // Create RoboEyes instance
  _eyes = new RoboEyes<Adafruit_SSD1306>(_display);
  
  // Initialize RoboEyes with screen dimensions and 60 FPS
  _eyes->begin(OLED_WIDTH, OLED_HEIGHT, 60);
  
  // Set autoblink and idle mode
  _eyes->setAutoblinker(true, 3, 2);  // Blink every 3±2 seconds
  _eyes->setIdleMode(true, 2, 2);     // Look around every 2±2 seconds
  
  // Start with eyes centered
  _eyes->setPosition(POS_CENTER);
  _eyesOpen = true;
  _active = true;
  
  DEBUG_PRINTLN("RoboEyesMode: Initialized");
  
  // Show help text briefly
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(WHITE);
  _display.setCursor(0, 0);
  _display.println("RoboEyes Mode");
  _display.println("1=Happy 2=Tired");
  _display.println("3=Angry A=Default");
  _display.println("4=Blink 5=Laugh");
  _display.println("6=Confused");
  _display.println("7-9,C=Directions");
  _display.println("Starting...");
  _display.display();
  delay(2000);
}

void RoboEyesMode::update() {
  if (_eyes && _active) {
    // Update eyes animation (framerate-limited internally)
    _eyes->update();
  }
}

void RoboEyesMode::handleKey(char key) {
  DEBUG_PRINTF("RoboEyesMode: Key pressed: %c\n", key);
  
  switch (key) {
    // Mood controls (row 1)
    case '1':  // Happy
      _eyes->setMood(HAPPY);
      DEBUG_PRINTLN("  Mood: HAPPY");
      break;
      
    case '2':  // Tired
      _eyes->setMood(TIRED);
      DEBUG_PRINTLN("  Mood: TIRED");
      break;
      
    case '3':  // Angry
      _eyes->setMood(ANGRY);
      DEBUG_PRINTLN("  Mood: ANGRY");
      break;
      
    case 'A':  // Default mood
      _eyes->setMood(DEFAULT);
      DEBUG_PRINTLN("  Mood: DEFAULT");
      break;
    
    // Animation controls (row 2)
    case '4':  // Blink
      _eyes->blink();
      DEBUG_PRINTLN("  Animation: BLINK");
      break;
      
    case '5':  // Laugh
      _eyes->anim_laugh();
      DEBUG_PRINTLN("  Animation: LAUGH");
      break;
      
    case '6':  // Confused
      _eyes->anim_confused();
      DEBUG_PRINTLN("  Animation: CONFUSED");
      break;
      
    case 'B':  // Toggle open/close
      if (_eyesOpen) {
        _eyes->close();
        _eyesOpen = false;
        DEBUG_PRINTLN("  Eyes: CLOSED");
      } else {
        _eyes->open();
        _eyesOpen = true;
        DEBUG_PRINTLN("  Eyes: OPENED");
      }
      break;
    
    // Position controls (row 3)
    case '7':  // Look North
      _eyes->setPosition(N);
      DEBUG_PRINTLN("  Position: NORTH");
      break;
      
    case '8':  // Look East
      _eyes->setPosition(E);
      DEBUG_PRINTLN("  Position: EAST");
      break;
      
    case '9':  // Look South
      _eyes->setPosition(S);
      DEBUG_PRINTLN("  Position: SOUTH");
      break;
      
    case 'C':  // Look West
      _eyes->setPosition(W);
      DEBUG_PRINTLN("  Position: WEST");
      break;
    
    // Mode toggles (row 4)
    case '*':  // Toggle Curious mode
      _curiousMode = !_curiousMode;
      _eyes->setCuriosity(_curiousMode);
      DEBUG_PRINTF("  Curious: %s\n", _curiousMode ? "ON" : "OFF");
      break;
      
    case '0':  // Toggle Idle mode
      _idleMode = !_idleMode;
      _eyes->setIdleMode(_idleMode, 2, 2);  // Random movement every 2±2 seconds
      DEBUG_PRINTF("  Idle: %s\n", _idleMode ? "ON" : "OFF");
      break;
      
    case '#':  // Toggle Sweat
      _sweatMode = !_sweatMode;
      _eyes->setSweat(_sweatMode);
      DEBUG_PRINTF("  Sweat: %s\n", _sweatMode ? "ON" : "OFF");
      break;
      
    case 'D':  // Toggle Cyclops mode
      _cyclopsMode = !_cyclopsMode;
      _eyes->setCyclops(_cyclopsMode);
      DEBUG_PRINTF("  Cyclops: %s\n", _cyclopsMode ? "ON" : "OFF");
      break;
  }
}

void RoboEyesMode::reset() {
  DEBUG_PRINTLN("RoboEyesMode: Reset");
  
  // Reset to defaults
  _eyes->setMood(DEFAULT);
  _eyes->setPosition(DEFAULT);
  _eyes->open();
  _eyes->setCuriosity(OFF);
  _eyes->setCyclops(OFF);
  _eyes->setIdleMode(OFF);
  _eyes->setSweat(OFF);
  _eyes->setAutoblinker(ON, 3, 2);
  
  _eyesOpen = true;
  _curiousMode = false;
  _cyclopsMode = false;
  _idleMode = false;
  _sweatMode = false;
}
