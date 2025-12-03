# ESP32-S3 Macropad Wiring Guide

**Canonical Wiring Documentation - Single Source of Truth**

This document defines the official pin mappings for the ESP32-S3 Macropad hardware. All code and documentation should reference this as the authoritative source.

---

## Board Information

- **Microcontroller**: ESP32-S3-DevKitC-1
- **Power Supply**: USB 5V or external 5V
- **Logic Level**: 3.3V

---

## Complete Pin Mapping Table

| Component          | Signal/Pin Name | ESP32-S3 GPIO | Notes                    |
|--------------------|-----------------|---------------|--------------------------|
| **OLED Display**   |                 |               | I2C Address: 0x3C        |
|                    | SDA             | GPIO 21       | I2C Data (default)       |
|                    | SCL             | GPIO 22       | I2C Clock (default)      |
|                    | VCC             | 3.3V          |                          |
|                    | GND             | GND           |                          |
| **Keypad Row 1**   | R1              | GPIO 4        | Row scan                 |
| **Keypad Row 2**   | R2              | GPIO 5        | Row scan                 |
| **Keypad Row 3**   | R3              | GPIO 6        | Row scan                 |
| **Keypad Row 4**   | R4              | GPIO 7        | Row scan                 |
| **Keypad Col 1**   | C1              | GPIO 15       | Column sense             |
| **Keypad Col 2**   | C2              | GPIO 16       | Column sense             |
| **Keypad Col 3**   | C3              | GPIO 17       | Column sense             |
| **Keypad Col 4**   | C4              | GPIO 18       | Column sense             |
| **Rotary Encoder** |                 |               |                          |
|                    | CLK (A)         | GPIO 8        | Rotation phase A         |
|                    | DT (B)          | GPIO 9        | Rotation phase B         |
|                    | SW (Button)     | GPIO 10       | Push button (active LOW) |
|                    | VCC             | 3.3V          |                          |
|                    | GND             | GND           |                          |
| **Status LEDs**    |                 |               | Built-in RGB LED         |
|                    | Red             | GPIO 47       | Common anode (inverted)  |
|                    | Green           | GPIO 21       | Common anode (inverted)  |
|                    | Blue            | GPIO 48       | Common anode (inverted)  |

---

## Detailed Component Wiring

### 1. OLED Display (SSD1306 128×64, I2C)

The OLED display uses the ESP32-S3's default I2C interface.

```
OLED Display          ESP32-S3
────────────────────────────────
VCC     ───────────→  3.3V
GND     ───────────→  GND
SDA     ───────────→  GPIO 21
SCL     ───────────→  GPIO 22
```

**Configuration:**
- **I2C Address**: 0x3C (default for most SSD1306 modules)
- **Pull-up Resistors**: Usually built-in on OLED module (4.7kΩ typical)
- **Bus Speed**: 400kHz (Fast Mode)

**Notes:**
- No external pull-up resistors required if module has them
- Verify I2C address with scanner if display doesn't work
- Some modules use 0x3D - check your specific module

---

### 2. 4×4 Matrix Keypad

The keypad is scanned using a row-column matrix configuration.

#### Row Connections (Output - Active LOW)
```
Keypad Row           ESP32-S3
────────────────────────────────
Row 1   ───────────→  GPIO 4
Row 2   ───────────→  GPIO 5
Row 3   ───────────→  GPIO 6
Row 4   ───────────→  GPIO 7
```

#### Column Connections (Input with Pull-up)
```
Keypad Column        ESP32-S3
────────────────────────────────
Column 1 ──────────→  GPIO 15
Column 2 ──────────→  GPIO 16
Column 3 ──────────→  GPIO 17
Column 4 ──────────→  GPIO 18
```

#### Keypad Layout
```
     Col1   Col2   Col3   Col4
     (15)   (16)   (17)   (18)
      │      │      │      │
Row1 ─┼──────┼──────┼──────┼─── (4)
      │  1   │  2   │  3   │  A
      │      │      │      │
Row2 ─┼──────┼──────┼──────┼─── (5)
      │  4   │  5   │  6   │  B
      │      │      │      │
Row3 ─┼──────┼──────┼──────┼─── (6)
      │  7   │  8   │  9   │  C
      │      │      │      │
Row4 ─┼──────┼──────┼──────┼─── (7)
      │  *   │  0   │  #   │  D
```

**Configuration:**
- **Debounce**: 10ms (configured in software)
- **Scan Rate**: ~10ms per cycle
- **Pull-up Resistors**: Internal pull-ups enabled on column pins

**Notes:**
- Row pins are configured as OUTPUT
- Column pins are configured as INPUT_PULLUP
- No external resistors required

---

### 3. Rotary Encoder (with Push Button)

The encoder provides rotation sensing and a push button.

```
Rotary Encoder       ESP32-S3
────────────────────────────────
CLK (A)  ──────────→  GPIO 8
DT  (B)  ──────────→  GPIO 9
SW  (Button) ──────→  GPIO 10
+   (VCC) ──────────→  3.3V
GND      ──────────→  GND
```

**Configuration:**
- **Type**: Quadrature encoder (2-bit Gray code)
- **Detents**: Typical encoders have 20 detents per revolution
- **Button**: Active LOW (pressed = LOW, released = HIGH)
- **Pull-up Resistors**: Internal pull-ups enabled on all pins
- **Debounce**: 50ms for button (software)

**Encoder States:**
```
Rotation CW:  00 → 01 → 11 → 10 → 00
Rotation CCW: 00 → 10 → 11 → 01 → 00
```

**Notes:**
- GPIO 8 and 9 monitor rotation (quadrature signals)
- GPIO 10 monitors button press
- Long press detection: 1000ms threshold
- No external components required

---

### 4. Status LEDs (Built-in RGB LED)

The ESP32-S3-DevKitC-1 has an onboard RGB LED with common anode configuration.

```
LED Color            ESP32-S3
────────────────────────────────
Red      ──────────→  GPIO 47
Green    ──────────→  GPIO 21
Blue     ──────────→  GPIO 48
Common Anode ──────→  3.3V (internal)
```

**Configuration:**
- **Type**: Common Anode (inverted logic)
- **Control**: LOW = ON, HIGH = OFF
- **Current**: Internally current-limited
- **Brightness**: PWM capable (0-255)

**LED Status Indicators:**
- **GREEN**: HID connected (USB/Bluetooth)
- **RED**: HID disconnected or error
- **BLUE**: Ready to pair / Waiting for connection

**Notes:**
- Inverted logic due to common anode configuration
- Use `digitalWrite(pin, LOW)` to turn LED ON
- Use `digitalWrite(pin, HIGH)` to turn LED OFF
- Safe to use without external resistors

---

## Power Distribution

```
USB 5V Input
    │
    ├──→ ESP32-S3 Voltage Regulator
    │        │
    │        └──→ 3.3V System Rail
    │                  │
    │                  ├──→ OLED Display (VCC)
    │                  ├──→ Rotary Encoder (+)
    │                  └──→ Internal Pull-ups
    │
    └──→ USB Data Lines (HID Communication)

GND: Common ground for all components
```

**Power Requirements:**
- **ESP32-S3**: ~500mA max (with WiFi/BLE)
- **OLED Display**: ~20mA typical
- **Total System**: <600mA
- **USB Power**: 5V, 500mA minimum

---

## Validation and Testing

### I2C Bus Test
```cpp
Wire.begin();
Wire.beginTransmission(0x3C);
byte error = Wire.endTransmission();
// error == 0 means device found
```

### Keypad Test
- Press each key and verify correct character in serial monitor
- Check for proper debouncing (no double-presses)

### Encoder Test
- Rotate and verify direction detection
- Press button and verify LOW state reading

### LED Test
```cpp
digitalWrite(LED_RED, LOW);    // Red ON
digitalWrite(LED_GREEN, LOW);  // Green ON
digitalWrite(LED_BLUE, LOW);   // Blue ON
```

---

## Troubleshooting

### OLED Display Not Working
1. Verify I2C address (try 0x3C or 0x3D)
2. Check SDA/SCL connections (no swap)
3. Ensure 3.3V power supply
4. Run I2C scanner code

### Keypad Not Responding
1. Verify row and column pin assignments
2. Check for loose connections
3. Test continuity of keypad matrix
4. Ensure internal pull-ups are enabled

### Encoder Issues
1. Check CLK and DT connections (don't swap)
2. Verify 3.3V power to encoder
3. Test button separately (should read HIGH when released)

### LED Not Lighting
1. Remember inverted logic (LOW = ON)
2. Check GPIO assignments
3. Verify GPIO 21 isn't conflicting with I2C SDA

---

## Schematic Notes

- All GPIOs operate at 3.3V logic level
- ESP32-S3 has internal pull-up/pull-down resistors (45kΩ typical)
- USB provides HID functionality without additional hardware
- No external crystal required (internal oscillator used)

---

## Software Pin Definitions

Reference these definitions in your code:

```cpp
// OLED Display (I2C)
#define SCREEN_ADDRESS 0x3C
// SDA: GPIO 21 (default Wire)
// SCL: GPIO 22 (default Wire)

// Keypad Matrix
byte rowPins[4] = {4, 5, 6, 7};
byte colPins[4] = {15, 16, 17, 18};

// Rotary Encoder
#define ENCODER_PIN_A 8
#define ENCODER_PIN_B 9
#define ENCODER_BTN 10

// Status LEDs (Common Anode - Inverted)
#define LED_RED    47
#define LED_GREEN  21
#define LED_BLUE   48
```

---

## Revision History

| Version | Date       | Changes                          |
|---------|------------|----------------------------------|
| 1.0     | 2025-12-03 | Initial wiring documentation     |

---

## Notes

- This document is the **single source of truth** for hardware wiring
- All code must reference these pin assignments
- Any wiring changes must be documented here first
- Test all connections before flashing firmware

