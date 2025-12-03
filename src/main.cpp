/*
 *  MACROPAD — ESP32-S3 + OLED + 4×4 Keypad + Encoder
 *  
 *  Profiles:
 *   - DEV: Cut, Copy, Paste, Copilot shortcuts
 *   - MEETING: Mic Mute, Video Toggle controls
 *
 *  Encoder:
 *   - Rotate (NORMAL) = volume up/down
 *   - Short press (NORMAL) = speaker mute
 *   - Long press = enter / handle menu
 *
 *  MENU:
 *   - Rotate = select DEV / MEETING
 *   - Short press = apply selection
 *   - Long press = exit without change
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Encoder.h>
#include <USB.h>
#include <USBHID.h>

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Keypad configuration (4x4 matrix)
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Define GPIO pins for keypad
byte rowPins[ROWS] = {4, 5, 6, 7};    // Row pins
byte colPins[COLS] = {15, 16, 17, 18}; // Column pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Rotary Encoder pins
#define ENCODER_PIN_A 8
#define ENCODER_PIN_B 9
#define ENCODER_BTN 10
Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

// Encoder button state tracking
unsigned long lastButtonPress = 0;
unsigned long buttonPressStart = 0;
bool buttonPressed = false;
bool longPressHandled = false;
#define LONG_PRESS_TIME 1000  // 1 second for long press
#define DEBOUNCE_TIME 50

// Profile system
enum Profile { DEV, MEETING };
Profile currentProfile = DEV;
Profile selectedProfile = DEV;

// Mode system
enum Mode { NORMAL, MENU };
Mode currentMode = NORMAL;

// Encoder position tracking
long lastEncoderPos = 0;

// USB HID Keyboard
#include "USB.h"
USBHID HID;

// HID Report descriptor for keyboard
static const uint8_t _hidReportDescriptor[] = {
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Kybrd/Keypad)
  0x19, 0xE0,        //   Usage Minimum (0xE0)
  0x29, 0xE7,        //   Usage Maximum (0xE7)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x81, 0x02,        //   Input (Data,Var,Abs)
  0x95, 0x01,        //   Report Count (1)
  0x75, 0x08,        //   Report Size (8)
  0x81, 0x03,        //   Input (Const,Var,Abs)
  0x95, 0x06,        //   Report Count (6)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x65,        //   Logical Maximum (101)
  0x05, 0x07,        //   Usage Page (Kybrd/Keypad)
  0x19, 0x00,        //   Usage Minimum (0x00)
  0x29, 0x65,        //   Usage Maximum (0x65)
  0x81, 0x00,        //   Input (Data,Array,Abs)
  0xC0,              // End Collection
  
  // Consumer Control (Volume)
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
  0x81, 0x00,        //   Input (Data,Array,Abs)
  0xC0               // End Collection
};

// HID key codes
#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LGUI   0x08

#define KEY_X 0x1B
#define KEY_C 0x06
#define KEY_V 0x19
#define KEY_I 0x0C

// Consumer control codes
#define VOLUME_UP      0xE9
#define VOLUME_DOWN    0xEA
#define MUTE           0xE2

// Structure for keyboard report
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

// Function to send keyboard key
void sendKey(uint8_t modifiers, uint8_t key) {
  KeyReport report = {0};
  report.modifiers = modifiers;
  report.keys[0] = key;
  
  HID.SendReport(1, &report, sizeof(report));
  delay(10);
  
  // Release
  memset(&report, 0, sizeof(report));
  HID.SendReport(1, &report, sizeof(report));
  delay(10);
}

// Function to send consumer control (volume)
void sendConsumerKey(uint16_t key) {
  uint8_t report[2];
  report[0] = key & 0xFF;
  report[1] = (key >> 8) & 0xFF;
  
  HID.SendReport(2, report, sizeof(report));
  delay(10);
  
  // Release
  report[0] = 0;
  report[1] = 0;
  HID.SendReport(2, report, sizeof(report));
  delay(10);
}

// Profile-specific key handlers
void handleDevProfile(char key) {
  switch(key) {
    case '1': // Cut
      sendKey(KEY_MOD_LCTRL, KEY_X);
      break;
    case '2': // Copy
      sendKey(KEY_MOD_LCTRL, KEY_C);
      break;
    case '3': // Paste
      sendKey(KEY_MOD_LCTRL, KEY_V);
      break;
    case '4': // Copilot (Ctrl+I in VS Code)
      sendKey(KEY_MOD_LCTRL, KEY_I);
      break;
    // Add more keys as needed
  }
}

void handleMeetingProfile(char key) {
  switch(key) {
    case '1': // Mic Mute (Ctrl+Shift+M for Teams/Zoom)
      sendKey(KEY_MOD_LCTRL | KEY_MOD_LSHIFT, 0x10); // M key
      break;
    case '2': // Video Toggle (Ctrl+Shift+O for Teams)
      sendKey(KEY_MOD_LCTRL | KEY_MOD_LSHIFT, 0x12); // O key
      break;
    // Add more keys as needed
  }
}

// Display functions
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  if (currentMode == NORMAL) {
    // Show current profile
    display.setCursor(0, 0);
    display.print("Mode: NORMAL");
    
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.print(currentProfile == DEV ? "DEV" : "MEETING");
    
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("Encoder: Vol +/-");
    display.setCursor(0, 52);
    display.print("Press: Mute");
  } else {
    // MENU mode - show selection
    display.setCursor(0, 0);
    display.print("SELECT PROFILE:");
    
    display.setTextSize(2);
    
    // Show DEV option
    display.setCursor(10, 20);
    if (selectedProfile == DEV) {
      display.print("> DEV");
    } else {
      display.print("  DEV");
    }
    
    // Show MEETING option
    display.setCursor(10, 40);
    if (selectedProfile == MEETING) {
      display.print("> MEET");
    } else {
      display.print("  MEET");
    }
  }
  
  display.display();
}

// Encoder handling
void handleEncoder() {
  long newPos = encoder.read() / 4; // Divide by 4 for better control
  
  if (newPos != lastEncoderPos) {
    if (currentMode == NORMAL) {
      // Volume control
      if (newPos > lastEncoderPos) {
        sendConsumerKey(VOLUME_UP);
      } else {
        sendConsumerKey(VOLUME_DOWN);
      }
    } else {
      // Menu navigation
      if (newPos > lastEncoderPos) {
        selectedProfile = MEETING;
      } else {
        selectedProfile = DEV;
      }
      updateDisplay();
    }
    lastEncoderPos = newPos;
  }
}

// Encoder button handling
void handleEncoderButton() {
  bool buttonState = digitalRead(ENCODER_BTN) == LOW; // Assuming active LOW
  
  if (buttonState && !buttonPressed) {
    // Button just pressed
    buttonPressStart = millis();
    buttonPressed = true;
    longPressHandled = false;
  } else if (!buttonState && buttonPressed) {
    // Button just released
    unsigned long pressDuration = millis() - buttonPressStart;
    buttonPressed = false;
    
    if (!longPressHandled && pressDuration < LONG_PRESS_TIME) {
      // Short press
      if (currentMode == NORMAL) {
        // Mute speaker
        sendConsumerKey(MUTE);
      } else {
        // Apply profile selection
        currentProfile = selectedProfile;
        currentMode = NORMAL;
        updateDisplay();
      }
    }
  } else if (buttonPressed && !longPressHandled) {
    // Check for long press
    if (millis() - buttonPressStart >= LONG_PRESS_TIME) {
      longPressHandled = true;
      
      if (currentMode == NORMAL) {
        // Enter menu
        currentMode = MENU;
        selectedProfile = currentProfile;
      } else {
        // Exit menu without change
        currentMode = NORMAL;
        selectedProfile = currentProfile;
      }
      updateDisplay();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Macropad Starting...");
  
  // Initialize encoder button pin
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  
  // Initialize HID
  HID.begin();
  HID.setReportDescriptor(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  USB.begin();
  
  // Initialize I2C and OLED
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Macropad Ready!");
  display.display();
  delay(2000);
  
  updateDisplay();
  
  Serial.println("Setup complete!");
}

void loop() {
  // Handle encoder rotation
  handleEncoder();
  
  // Handle encoder button
  handleEncoderButton();
  
  // Handle keypad
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);
    
    if (currentMode == NORMAL) {
      if (currentProfile == DEV) {
        handleDevProfile(key);
      } else {
        handleMeetingProfile(key);
      }
    }
  }
  
  delay(10);
}
