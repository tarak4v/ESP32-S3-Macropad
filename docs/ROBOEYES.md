# RoboEyes Mode - Animated Display Feature

## Overview

RoboEyes mode transforms your ESP32-S3 Macropad into an interactive animated robot eye display using the [FluxGarage RoboEyes library](https://github.com/FluxGarage/RoboEyes). The 128x64 OLED displays smooth, expressive robot eyes that respond to keypad inputs.

## Activation

**Toggle RoboEyes Mode**: **Long press the encoder button (1+ second)**

- From Normal mode → RoboEyes mode (eyes appear)
- From RoboEyes mode → Normal mode (return to HID functionality)

## Keypad Controls

### Mood Expressions (Row 1)
- **`1`** - **Happy** 😊 (smiling eyes with curved bottom)
- **`2`** - **Tired** 😴 (half-closed with droopy eyelids)
- **`3`** - **Angry** 😠 (sharp angled eyelids)
- **`A`** - **Default** 😐 (neutral expression)

### Animations (Row 2)
- **`4`** - **Blink** (quick eye blink)
- **`5`** - **Laugh** (eyes shake up and down for 500ms)
- **`6`** - **Confused** (eyes shake left and right for 500ms)
- **`B`** - **Toggle Open/Close** (manually open or close eyes)

### Eye Position (Row 3)
- **`7`** - **Look North** (eyes look up)
- **`8`** - **Look East** (eyes look right)
- **`9`** - **Look South** (eyes look down)
- **`C`** - **Look West** (eyes look left)

Additional positions:
- **NE** (northeast), **SE** (southeast), **SW** (southwest), **NW** (northwest) can be accessed programmatically

### Special Modes (Row 4)
- **`*`** - **Toggle Curious Mode** (outer eye gets larger when looking left/right)
- **`0`** - **Toggle Idle Mode** (eyes randomly wander every 2±2 seconds)
- **`#`** - **Toggle Sweat** (animated sweat drops appear when stressed)
- **`D`** - **Toggle Cyclops Mode** (show only one eye)

## Features

### Automatic Behaviors
- **Auto-Blink**: Eyes blink automatically every 3±2 seconds
- **Smooth Animations**: 60 FPS animation with tweened movements
- **Expressive Design**: Rounded eyes with customizable border radius

### Eye Configuration
- **Eye Size**: 36×36 pixels per eye
- **Space Between**: 10 pixels
- **Border Radius**: 8 pixels (rounded corners)
- **Frame Rate**: 60 FPS (limited internally to prevent overdraw)

### Visual Feedback
- All keypad presses are logged to serial monitor
- Eye expressions change instantly
- Animations play for defined durations

## Examples

### Create a Happy, Curious Robot
1. Long press encoder → Enter RoboEyes mode
2. Press `1` → Happy mood (smiling)
3. Press `*` → Enable curious mode
4. Press `8` → Look East (right eye gets larger)
5. Press `0` → Enable idle mode (eyes wander)

### Stressed Robot with Sweat
1. Press `3` → Angry mood
2. Press `#` → Enable sweat drops
3. Press `6` → Play confused animation
4. Watch the animated sweat drops fall!

### Cyclops Mode
1. Press `D` → Toggle cyclops (single eye)
2. Press `7-9,C` → Move the eye in different directions
3. Press `5` → Laugh animation

### Tired Robot Falling Asleep
1. Press `2` → Tired mood (droopy eyelids)
2. Press `4` → Manual blink
3. Press `B` → Close eyes (sleep)
4. Press `B` again → Open eyes (wake up)

## Technical Details

### Library Integration
- **Library**: [FluxGarage/RoboEyes](https://github.com/FluxGarage/RoboEyes)
- **Display**: Adafruit SSD1306 (128×64 monochrome OLED)
- **Colors**: 0=Black background, 1=White foreground

### Animation System
- **Frame Limiting**: Internal timer ensures max 60 FPS
- **Tweening**: Smooth transitions between eye positions/sizes
- **Macro Animations**: Pre-built sequences (laugh, confused, sweat)

### Memory Usage
- **Heap**: RoboEyes instance dynamically allocated after POST
- **Flash**: ~15KB for RoboEyes library code
- **Stack**: Minimal (operates on existing display buffer)

### Performance
- **CPU**: <5% at 60 FPS (animation update is lightweight)
- **Display**: Full refresh every 16.67ms in RoboEyes mode
- **Keypad**: Scanned every 10ms with debounce

## Customization

### Modify Eye Appearance
Edit `roboeyes_mode.cpp` in the `begin()` function:

```cpp
// Change eye size
_eyes.setWidth(40, 40);      // Wider eyes
_eyes.setHeight(30, 30);     // Shorter eyes

// Adjust spacing
_eyes.setSpacebetween(5);    // Closer together

// Change shape
_eyes.setBorderradius(12, 12); // More rounded
```

### Add More Positions
```cpp
case '5':  // Custom: Look Northeast
  _eyes.setPosition(NE);
  break;
```

### Adjust Animation Timing
```cpp
// Faster auto-blink (every 2±1 seconds)
_eyes.setAutoblinker(ON, 2, 1);

// More active idle mode (every 1±1 second)
_eyes.setIdleMode(ON, 1, 1);
```

### Add Horizontal/Vertical Flicker
```cpp
// Add shaky/nervous effect
_eyes.setHFlicker(ON, 3);  // Horizontal shake, 3px amplitude
_eyes.setVFlicker(ON, 2);  // Vertical shake, 2px amplitude
```

## Troubleshooting

### Eyes Don't Appear
- Check serial monitor for initialization messages
- Ensure OLED is working (test with POST)
- Verify RoboEyes library is installed in platformio.ini

### Choppy Animation
- Reduce frame rate in `begin()`: `_eyes.begin(OLED_WIDTH, OLED_HEIGHT, 30);`
- Disable idle mode to reduce computational load
- Check for excessive serial printing

### Keypad Not Responding
- Verify in Normal mode that keypad works
- Check encoder button long press detection
- Serial monitor shows key presses when debugging

### Can't Exit RoboEyes Mode
- Long press encoder button (hold for 1+ second)
- If stuck, reset device (power cycle)

## Future Enhancements

Possible improvements:
- **Profile Integration**: Save eye expressions to JSON keymaps
- **Encoder Control**: Rotate encoder to adjust eye size/spacing
- **Temperature Display**: Show temperature on forehead (with sensor)
- **Sound Reactive**: Eyes respond to microphone input
- **Sleep Mode**: Auto-close eyes after inactivity timeout

## Credits

- **RoboEyes Library**: [FluxGarage](https://github.com/FluxGarage) (GNU GPL v3)
- **Display Driver**: Adafruit SSD1306 library
- **Integration**: ESP32-S3 Macropad firmware

## Related Documentation

- [RoboEyes GitHub Repository](https://github.com/FluxGarage/RoboEyes)
- [RoboEyes YouTube Tutorial](https://www.youtube.com/@FluxGarage)
- [Adafruit GFX Graphics Primitives](https://learn.adafruit.com/adafruit-gfx-graphics-library)

---

**Have fun making expressive robot eyes! 🤖👀**
