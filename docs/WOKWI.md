# Wokwi Simulation Guide

## Overview

The ESP32-S3 Macropad project is configured for simulation using [Wokwi](https://wokwi.com/), an online electronics simulator that supports ESP32 boards and common peripherals.

## Hardware Configuration

The simulation includes:

- **ESP32-S3-DevKitC-1** - Main microcontroller
- **SSD1306 OLED Display** (128×64, I2C at 0x3C)
  - SDA: GPIO21
  - SCL: GPIO22
- **4×4 Membrane Keypad**
  - Rows: GPIO4, GPIO5, GPIO6, GPIO7
  - Columns: GPIO15, GPIO16, GPIO17, GPIO18
- **KY-040 Rotary Encoder**
  - CLK: GPIO8
  - DT: GPIO9
  - SW: GPIO10

## Running the Simulation

### Option 1: PlatformIO CLI

```bash
# Build the firmware
pio run -e esp32-s3-devkitc-1

# Run with Wokwi CLI (if installed)
wokwi-cli --timeout 30000 .
```

### Option 2: Wokwi VS Code Extension

1. Install the [Wokwi Simulator extension](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)
2. Open the Command Palette (Ctrl+Shift+P / Cmd+Shift+P)
3. Run: `Wokwi: Start Simulator`

### Option 3: Wokwi Web (Manual Upload)

1. Build the project: `pio run -e esp32-s3-devkitc-1`
2. Navigate to [Wokwi ESP32 Simulator](https://wokwi.com/projects/new/esp32-s3-devkitc-1)
3. Copy the contents of `diagram.json` to the Wokwi diagram editor
4. Upload `.pio/build/esp32-s3-devkitc-1/firmware.bin`

## Testing Features

### Normal Mode
1. Press keypad keys `1-9, A-D, *, #, 0` to send HID keyboard commands
2. Rotate encoder to control volume
3. Short press encoder button to mute/unmute speaker
4. Long press encoder button (>1 second) to enter menu

### RoboEyes Mode
1. Press `A` key to enter RoboEyes animated display mode
2. Use keypad to control eye expressions:
   - `1-4`: Moods (Happy, Tired, Angry, Default)
   - `5-9, 0, A, C`: Eye positions (N, E, S, W, etc.)
   - `B`: Blink
   - `*`: Curious mode toggle
   - `#`: Sweat animation
   - `D`: Cyclops mode toggle
3. Press `B` key to exit back to normal mode

### Menu System
1. Long press encoder button
2. Rotate encoder to navigate menu
3. Short press to select
4. Available options: Profile selection, Settings, About

## Debugging

The simulation runs with `CORE_DEBUG_LEVEL=3`, providing detailed serial output at 115200 baud.

### Serial Monitor Commands

```bash
# Monitor serial output
pio device monitor -e esp32-s3-devkitc-1
```

### Expected Boot Sequence

```
====================================
    ESP32-S3 MACROPAD v1.0.0
====================================
Firmware: Tarak-HID
Author  : tarak4v
Date    : 2025-12-05
====================================

[BOOT] Power-On Self Test (POST)
✓ Display initialized
✓ Keypad initialized
✓ Encoder initialized
✓ USB HID initialized
✓ Storage initialized
✓ Configuration loaded
✓ RoboEyes mode ready (Press 'A' to activate)

[READY] System initialized successfully
```

## Limitations

Wokwi simulation has some limitations compared to physical hardware:

1. **USB HID**: Simulated, not actual USB communication
2. **WiFi/OTA**: Not supported (OTA_ENABLED=false by default)
3. **RGB LED**: ESP32-S3's built-in LED may not be visible
4. **Performance**: Simulation is slower than real hardware
5. **Timing**: Some timing-dependent features may behave differently

## File Structure

```
ESP32-S3-Macropad/
├── wokwi.toml           # Wokwi configuration
├── diagram.json         # Hardware wiring diagram
├── platformio.ini       # Build configuration (includes wokwi env)
└── docs/
    └── WOKWI.md        # This file
```

## Troubleshooting

### Build Fails
- Ensure all libraries are installed: `pio lib install`
- Clean build: `pio run -t clean`
- Rebuild: `pio run -e esp32-s3-devkitc-1`

### Display Not Working
- Check I2C address in `Config.h` matches `diagram.json` (0x3C)
- Verify GPIO pins (SDA=21, SCL=22)

### Keypad Not Responding
- Ensure row/column GPIO pins match between `Config.h` and `diagram.json`
- Check keypad matrix layout

### Encoder Not Working
- Verify CLK, DT, SW pins in both files
- Check encoder library is installed

## Resources

- [Wokwi Documentation](https://docs.wokwi.com/)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [Project GitHub](https://github.com/tarak4v/ESP32-S3-Macropad)

## License

See main project README for license information.
