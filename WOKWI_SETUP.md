# Wokwi Configuration Complete ✅

## Files Created

1. **wokwi.toml** - Wokwi simulator configuration
   - Points to firmware.elf
   - Configures serial monitor (115200 baud)
   - Enables GDB debugging

2. **diagram.json** - Hardware wiring diagram
   - ESP32-S3-DevKitC-1 board
   - SSD1306 OLED (128×64, I2C 0x3C)
   - 4×4 Membrane Keypad
   - KY-040 Rotary Encoder
   - All GPIO connections mapped

3. **docs/WOKWI.md** - Complete simulation guide
   - Hardware configuration details
   - Running instructions (3 methods)
   - Testing procedures
   - Debugging tips
   - Troubleshooting guide

4. **wokwi-start.ps1** - Quick start script
   - Automated build and simulation
   - Checks for Wokwi CLI
   - Provides alternative methods

5. **platformio.ini** - Added `[env:wokwi]` environment
   - Wokwi-specific build configuration
   - WOKWI_SIMULATION flag

## Hardware Mapping

### OLED Display (I2C)
- VCC → ESP32-S3 3V3
- GND → ESP32-S3 GND
- SDA → GPIO21
- SCL → GPIO22
- Address: 0x3C

### 4×4 Keypad Matrix
**Rows:**
- R1 → GPIO4
- R2 → GPIO5
- R3 → GPIO6
- R4 → GPIO7

**Columns:**
- C1 → GPIO15
- C2 → GPIO16
- C3 → GPIO17
- C4 → GPIO18

**Layout:**
```
1  2  3  A
4  5  6  B
7  8  9  C
*  0  #  D
```

### Rotary Encoder (KY-040)
- VCC → ESP32-S3 3V3
- GND → ESP32-S3 GND
- CLK → GPIO8
- DT → GPIO9
- SW → GPIO10

## How to Run

### Method 1: PowerShell Script (Easiest)
```powershell
.\wokwi-start.ps1
```

### Method 2: Manual PlatformIO + Wokwi CLI
```bash
pio run -e esp32-s3-devkitc-1
wokwi-cli --timeout 60000 .
```

### Method 3: VS Code Extension
1. Install "Wokwi Simulator" extension
2. Press F1 → "Wokwi: Start Simulator"

### Method 4: Web Interface
1. Build: `pio run -e esp32-s3-devkitc-1`
2. Go to https://wokwi.com/projects/new/esp32-s3-devkitc-1
3. Copy `diagram.json` contents to editor
4. Upload `.pio/build/esp32-s3-devkitc-1/firmware.bin`

## Features to Test in Simulation

### Boot Sequence
- Watch serial output for POST (Power-On Self Test)
- Verify all modules initialize successfully
- Check for "RoboEyes mode ready" message

### Normal HID Mode
- Press keypad keys → sends keyboard commands
- Rotate encoder → volume control
- Short press encoder → speaker mute
- Long press encoder → enter menu

### RoboEyes Mode
- Press 'A' → enter animated eyes mode
- Keys 1-4 → change moods
- Keys 5-9,0,A,C → eye positions
- Key B → blink
- Keys *,#,D → special effects
- Press 'B' → exit to normal mode

### Menu System
- Long press encoder button
- Rotate to navigate
- Short press to select

## Limitations

- USB HID is simulated (not actual USB)
- WiFi/OTA not available (disabled)
- RGB LED may not be visible
- Timing may differ from real hardware
- Performance is slower than physical device

## Next Steps

1. **Run simulation**: `.\wokwi-start.ps1`
2. **Test keypad**: Try all 16 keys
3. **Test encoder**: Rotate and press button
4. **Test RoboEyes**: Press 'A' to enter mode
5. **Debug**: Use serial monitor (115200 baud)

## Troubleshooting

**Build fails?**
```bash
pio lib install
pio run -t clean
pio run -e esp32-s3-devkitc-1
```

**Display blank?**
- Check I2C address (0x3C)
- Verify SDA/SCL pins (21/22)

**Keys not working?**
- Verify GPIO pin mapping
- Check keypad matrix layout

## Resources

- [Wokwi Documentation](https://docs.wokwi.com/)
- [Full Guide](docs/WOKWI.md)
- [Project GitHub](https://github.com/tarak4v/ESP32-S3-Macropad)
