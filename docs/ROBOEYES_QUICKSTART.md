# RoboEyes Mode - Quick Start Guide

##  What is RoboEyes Mode?

Transform your ESP32-S3 Macropad's OLED display into an interactive animated robot face! The eyes blink, look around, show emotions, and respond to every keypad press.

## 🚀 Getting Started

### Enter RoboEyes Mode
**Long press the encoder button** (hold for 1+ second)
- The POST test screen will disappear
- Animated robot eyes will appear
- All keypad buttons now control the eyes

### Exit RoboEyes Mode  
**Long press the encoder button again** (hold for 1+ second)
- Returns to normal HID keyboard mode

---

## 🎮 Keypad Controls

```
┌────┬────┬────┬────┐
│ 1  │ 2  │ 3  │ A  │  ← Moods
│😊  │😴  │😠  │😐  │
├────┼────┼────┼────┤
│ 4  │ 5  │ 6  │ B  │  ← Animations
│👁️  │😂  │😵  │👁️ │
├────┼────┼────┼────┤
│ 7  │ 8  │ 9  │ C  │  ← Directions
│ ↑  │ →  │ ↓  │ ←  │
├────┼────┼────┼────┤
│ *  │ 0  │ #  │ D  │  ← Special
│👀  │🔀  │💦  │👁  │
└────┴────┴────┴────┘
```

### Row 1: Moods 😊
- **1** = Happy (smiling eyes)
- **2** = Tired (droopy eyelids)
- **3** = Angry (sharp eyelids)
- **A** = Default (neutral)

### Row 2: Animations  🎬
- **4** = Blink once
- **5** = Laugh (shake up/down)
- **6** = Confused (shake left/right)
- **B** = Toggle open/close eyes

### Row 3: Look Around 👀
- **7** = Look up (North)
- **8** = Look right (East)
- **9** = Look down (South)
- **C** = Look left (West)

### Row 4: Special Modes ✨
- **\*** = Toggle curious mode (eyes enlarge when looking sideways)
- **0** = Toggle idle mode (eyes wander randomly)
- **#** = Toggle sweat drops (stress effect)
- **D** = Toggle cyclops mode (one eye only)

---

## 🎯 Try These Combos!

### Sleepy Robot
```
Press: 2 → B
Result: Tired eyes that close (falling asleep)
```

### Excited Robot
```
Press: 1 → * → 0
Result: Happy eyes that look around curiously
```

### Stressed Robot
```
Press: 3 → # → 6
Result: Angry eyes with sweat and confusion animation
```

### One-Eyed Monster
```
Press: D → 8 → 5
Result: Single eye looking right and laughing
```

---

## 💡 Features

✅ **60 FPS Animation** - Buttery smooth eye movements  
✅ **Auto-Blink** - Eyes blink naturally every 3±2 seconds  
✅ **Smooth Tweening** - All movements are interpolated  
✅ **Expressive** - Multiple moods and emotions  
✅ **Interactive** - Instant response to keypad input  
✅ **Customizable** - Modify code for different eye styles  

---

## 🎨 Customization

Want different eye sizes or shapes? Edit `roboeyes_mode.cpp`:

```cpp
// In begin() function:
_eyes.setWidth(40, 40);      // Make eyes wider
_eyes.setHeight(30, 30);     // Make eyes shorter  
_eyes.setBorderradius(12, 12); // More rounded
_eyes.setSpacebetween(5);    // Closer together
```

---

## 🐛 Troubleshooting

**Eyes don't appear?**
- Check serial monitor for "RoboEyes mode available" message
- Try resetting the device

**Keypad not responding?**
- Make sure you're in RoboEyes mode (long press encoder)
- Check if keys work in normal mode first

**Can't exit?**
- Hold encoder button for full 1+ second
- If stuck, power cycle the device

---

## 📚 Learn More

- Full documentation: `docs/ROBOEYES.md`
- RoboEyes library: https://github.com/FluxGarage/RoboEyes
- Video tutorials: https://youtube.com/@FluxGarage

---

**Have fun creating robot expressions! 🤖👁️👁️**
