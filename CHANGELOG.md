# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Modular firmware architecture with clean separation of concerns
- OLED menu system with navigation
- Persistent configuration storage
- Wokwi simulation support
- Comprehensive documentation

## [0.2.0] - 2025-12-03

### Added
- Keymap configuration system (keymaps/default.json)
- Complete HID keycode and modifier reference tables
- Consumer control codes (volume, media)
- Canonical wiring documentation (docs/wiring.md)
- GitHub Actions CI for build regression testing
- Firmware size reporting in build summary
- Build status badge in README
- Contributing guidelines (CONTRIBUTING.md)
- Code of Conduct (CODE_OF_CONDUCT.md)
- MIT License

### Changed
- Updated README with keymap configuration reference
- Simplified pin configuration in README to reference wiring.md

## [0.1.0] - 2025-12-03

### Added
- POST (Power On Self Test) with comprehensive hardware testing
- I2C bus validation
- OLED display initialization test
- Keypad matrix testing (all 16 keys)
- Rotary encoder rotation and button testing
- USB HID initialization verification
- LED status indicators (RGB LED on ESP32-S3)
  - GREEN = HID connected (USB/Bluetooth)
  - RED = HID disconnected
  - BLUE = Ready to pair / waiting for connection
- Real-time HID connection monitoring (every 500ms)
- Display shows test results and connection status
- Backup of original macropad code (main_backup.cpp)
- PlatformIO upload task configuration

### Fixed
- Git configuration for repository commits

## [0.0.1] - 2025-12-03

### Added
- Initial project structure
- ESP32-S3 USB HID support
- OLED display (SSD1306 128×64)
- 4×4 matrix keypad support
- Rotary encoder with push button
- Two profiles: DEV and MEETING
- Profile switching via encoder menu
- Volume control via encoder rotation
- Speaker mute on encoder button press
- GitHub Copilot instructions for development

### Profiles

#### DEV Profile
- Key 1: Cut (Ctrl+X)
- Key 2: Copy (Ctrl+C)
- Key 3: Paste (Ctrl+V)
- Key 4: Copilot (Ctrl+I in VS Code)

#### MEETING Profile
- Key 1: Mic Mute (Ctrl+Shift+M)
- Key 2: Video Toggle (Ctrl+Shift+O)

### Technical Details
- Platform: ESP32-S3-DevKitC-1
- Framework: Arduino
- Build System: PlatformIO
- Language: C++

[Unreleased]: https://github.com/tarak4v/ESP32-S3-Macropad/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/tarak4v/ESP32-S3-Macropad/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/tarak4v/ESP32-S3-Macropad/compare/v0.0.1...v0.1.0
[0.0.1]: https://github.com/tarak4v/ESP32-S3-Macropad/releases/tag/v0.0.1
