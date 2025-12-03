# ESP32-S3 Macropad

[![PlatformIO CI](https://github.com/tarak4v/ESP32-S3-Macropad/actions/workflows/build.yml/badge.svg)](https://github.com/tarak4v/ESP32-S3-Macropad/actions/workflows/build.yml)

A versatile USB HID macropad built with ESP32-S3, featuring OLED display, 4×4 matrix keypad, and rotary encoder with multiple profiles for different workflows.

## Features

- **Multiple Profiles**: Switch between DEV and MEETING modes
- **USB HID Support**: Acts as a native USB keyboard and media controller
- **OLED Display**: 128×64 SSD1306 display for visual feedback
- **4×4 Keypad**: 16 programmable keys per profile
- **Rotary Encoder**: Volume control and menu navigation
- **Profile Switching**: Easy profile selection via encoder menu

## Hardware Requirements

### Components
- ESP32-S3-DevKitC-1 development board
- SSD1306 128×64 OLED display (I2C)
- 4×4 matrix keypad
- Rotary encoder with push button
- Connecting wires

### Pin Configuration

> **📋 Complete wiring documentation**: See [docs/wiring.md](docs/wiring.md) for the canonical pin mapping reference.

**Quick Reference:**
- **OLED**: GPIO 21 (SDA), GPIO 22 (SCL) - I2C 0x3C
- **Keypad Rows**: GPIO 4, 5, 6, 7
- **Keypad Columns**: GPIO 15, 16, 17, 18
- **Encoder**: GPIO 8 (CLK), GPIO 9 (DT), GPIO 10 (Button)
- **Status LEDs**: GPIO 47 (Red), GPIO 21 (Green), GPIO 48 (Blue)

## Software Setup

### Prerequisites
- [VS Code](https://code.visualstudio.com/)
- [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)

### Installation

1. Clone or download this repository
2. Open the project folder in VS Code
3. PlatformIO will automatically install dependencies
4. Connect your ESP32-S3 board via USB
5. Build and upload the firmware

### Building the Project

Using PlatformIO in VS Code:
1. Click the PlatformIO icon in the sidebar
2. Select "Build" to compile
3. Select "Upload" to flash to the board
4. Select "Monitor" to view serial output

Or use the command palette (Ctrl+Shift+P):
- `PlatformIO: Build`
- `PlatformIO: Upload`
- `PlatformIO: Serial Monitor`

## Usage

### Normal Mode

**Encoder:**
- **Rotate**: Volume Up/Down
- **Short Press**: Speaker Mute
- **Long Press (1s)**: Enter Profile Menu

**Keypad (DEV Profile):**
- Key 1: Cut (Ctrl+X)
- Key 2: Copy (Ctrl+C)
- Key 3: Paste (Ctrl+V)
- Key 4: Copilot (Ctrl+I - VS Code)

**Keypad (MEETING Profile):**
- Key 1: Mic Mute (Ctrl+Shift+M)
- Key 2: Video Toggle (Ctrl+Shift+O)

### Menu Mode

**Encoder:**
- **Rotate**: Navigate between DEV and MEETING profiles
- **Short Press**: Apply selected profile and return to Normal mode
- **Long Press (1s)**: Exit menu without changing profile

## Profiles

### DEV Profile
Designed for software development:
- Quick access to clipboard operations
- GitHub Copilot activation
- Easily extendable for additional IDE shortcuts

### MEETING Profile
Optimized for video conferencing:
- Microphone mute/unmute
- Camera toggle
- Compatible with Teams, Zoom, and other platforms

## Customization

### Adding New Keys

Edit the key handlers in `src/main.cpp`:

```cpp
void handleDevProfile(char key) {
  switch(key) {
    case '1': // Your custom action
      sendKey(KEY_MOD_LCTRL, KEY_X);
      break;
    // Add more cases
  }
}
```

### Modifying Keyboard Shortcuts

Use the `sendKey()` function with modifiers:
- `KEY_MOD_LCTRL`: Ctrl
- `KEY_MOD_LSHIFT`: Shift
- `KEY_MOD_LALT`: Alt
- `KEY_MOD_LGUI`: Windows/Command key

### Adding New Profiles

1. Add profile to the enum in `main.cpp`:
```cpp
enum Profile { DEV, MEETING, YOUR_PROFILE };
```

2. Create a handler function:
```cpp
void handleYourProfile(char key) {
  // Handle keys for your profile
}
```

3. Update the display and handler logic

## Troubleshooting

### Device Not Recognized
- Ensure USB cable supports data transfer
- Check that `ARDUINO_USB_MODE=1` is set in `platformio.ini`
- Try a different USB port

### Keys Not Working
- Verify pin connections match the configuration
- Check serial monitor for debug output
- Test keypad independently

### Display Issues
- Confirm I2C address (default: 0x3C)
- Check SDA/SCL connections
- Test display with I2C scanner

## Dependencies

- **Adafruit GFX Library**: Graphics primitives
- **Adafruit SSD1306**: OLED display driver
- **Keypad**: Matrix keypad scanning
- **Encoder**: Rotary encoder handling
- **USB/USBHID**: ESP32-S3 USB HID support

## License

This project is open source and available for modification and redistribution.

## Credits

Built with PlatformIO and Arduino framework for ESP32-S3.
