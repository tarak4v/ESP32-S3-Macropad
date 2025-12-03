# ESP32-S3 Macropad Project Instructions

## Project Overview
- **Hardware**: ESP32-S3 microcontroller with OLED display, 4×4 matrix keypad, and rotary encoder
- **Framework**: PlatformIO with Arduino framework
- **Language**: C++
- **Features**: Multiple profiles (DEV, MEETING), HID keyboard, volume control, profile menu

## Development Guidelines
- Use PlatformIO for ESP32-S3 development
- Follow Arduino framework conventions
- Keep profile configurations modular and easy to extend
- Implement proper debouncing for encoder and keypad inputs
- Use Adafruit libraries for OLED display control

## Project Structure
- `src/` - Main firmware source code
- `include/` - Header files
- `lib/` - Custom libraries
- `platformio.ini` - PlatformIO configuration

## Hardware Configuration
- **Board**: ESP32-S3-DevKitC-1
- **Display**: SSD1306 128x64 OLED (I2C)
- **Keypad**: 4×4 matrix (8 GPIO pins)
- **Encoder**: Rotary encoder with push button (3 GPIO pins)

## Feature Requirements
### Profiles
- **DEV**: Cut, Copy, Paste, Copilot shortcuts
- **MEETING**: Mic Mute, Video Toggle controls

### Encoder Behavior
- **Normal Mode**:
  - Rotate: Volume up/down
  - Short press: Speaker mute
  - Long press: Enter menu
- **Menu Mode**:
  - Rotate: Select profile (DEV/MEETING)
  - Short press: Apply selection
  - Long press: Exit without change

## Status: ✅ Step 1 Complete - copilot-instructions.md created
