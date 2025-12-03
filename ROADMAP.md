# Roadmap

This document outlines the planned features and improvements for the ESP32-S3 Macropad project.

## Version 1.0.0 (Current Sprint) 🚧

**Goal:** Stable, modular foundation with menu system

### Core Features
- [x] USB HID keyboard support
- [x] 4×4 matrix keypad
- [x] Rotary encoder with button
- [x] OLED display (128×64)
- [x] Multiple profiles (DEV, MEETING)
- [x] Volume control via encoder
- [x] Profile switching menu
- [ ] Persistent configuration (NVS)
- [ ] OLED menu navigation
- [ ] Settings menu

### Infrastructure
- [x] GitHub Actions CI
- [x] Build automation
- [x] Keymap JSON configuration
- [x] Canonical wiring documentation
- [x] Contributing guidelines
- [ ] Modular firmware architecture
- [ ] Unit testing framework
- [ ] Wokwi simulation support

## Version 1.1.0 (Q1 2026) 📅

**Goal:** Enhanced user experience and customization

### Features
- [ ] Profile manager UI
- [ ] On-device keymap editor
- [ ] Multiple keymap support
- [ ] Brightness control
- [ ] Debounce configuration
- [ ] Long press time adjustment
- [ ] Factory reset option
- [ ] Profile colors/themes

### Quality of Life
- [ ] Smooth UI transitions
- [ ] Loading indicators
- [ ] Error messages
- [ ] Status notifications
- [ ] Battery level indicator (if powered separately)

## Version 1.2.0 (Q2 2026) 📅

**Goal:** Advanced input and macros

### Features
- [ ] Multi-key combinations
- [ ] Macro recording
- [ ] Macro playback
- [ ] Delays in macros
- [ ] Text strings as macros
- [ ] Conditional macros
- [ ] Layer system (Fn key)

### Display Enhancements
- [ ] Custom icons
- [ ] Animations
- [ ] Profile preview
- [ ] Key legends display
- [ ] Activity indicators

## Version 2.0.0 (Q3 2026) 📅

**Goal:** Wireless connectivity and web configuration

### WiFi Features
- [ ] WiFi configuration UI
- [ ] Web-based configuration portal
- [ ] OTA firmware updates
- [ ] Keymap upload via WiFi
- [ ] Cloud profile sync (optional)

### Bluetooth Features
- [ ] Bluetooth HID support
- [ ] Multiple device pairing
- [ ] Device switching
- [ ] Battery status over BLE

### Web Interface
- [ ] Responsive web UI
- [ ] Real-time preview
- [ ] Drag-and-drop keymap editor
- [ ] Profile import/export
- [ ] Firmware update page

## Version 2.1.0 (Q4 2026) 📅

**Goal:** Integration and ecosystem

### Integrations
- [ ] Home Assistant integration
- [ ] MQTT support
- [ ] REST API
- [ ] WebSocket for real-time updates
- [ ] Plugin system

### Desktop Applications
- [ ] Desktop configurator (Electron)
- [ ] Key tester utility
- [ ] Macro recorder
- [ ] Profile manager

### Mobile App
- [ ] Android configuration app
- [ ] iOS configuration app
- [ ] QR code device pairing

## Version 3.0.0 (2027) 🔮

**Goal:** Advanced features and AI

### Display Upgrades
- [ ] Color OLED support
- [ ] Larger display support
- [ ] Touchscreen support (if available)
- [ ] E-ink support for always-on display

### Advanced Features
- [ ] Context-aware profiles (auto-switch based on active window)
- [ ] AI-powered macro suggestions
- [ ] Usage analytics and statistics
- [ ] Gesture recognition (encoder patterns)
- [ ] Voice commands (with mic addon)

### Hardware Extensions
- [ ] Additional encoder support
- [ ] RGB backlit keys
- [ ] Haptic feedback
- [ ] Modular addon system

---

## Community Requests

Features requested by the community (upvote on GitHub Discussions):

1. **More Profiles** (Requested by users)
   - Gaming profiles
   - Streaming controls  
   - Photo editing shortcuts
   - Video editing shortcuts

2. **Better Visual Feedback**
   - Key press animations
   - Profile change animations
   - Status icons

3. **Power Management**
   - Sleep mode
   - Power saving options
   - Wake on key press

4. **Accessibility**
   - Screen reader support
   - High contrast mode
   - Larger fonts option
   - Audio feedback

---

## Long-Term Vision

### Ultimate Goals
1. **Ease of Use**: Anyone can configure without coding
2. **Flexibility**: Supports any workflow
3. **Reliability**: Works flawlessly every time
4. **Extensibility**: Easy to add new features
5. **Community**: Vibrant ecosystem of keymaps and plugins

### Success Metrics
- 1000+ GitHub stars
- 100+ community-contributed keymaps
- 50+ contributors
- 10,000+ downloads
- Active community forum

---

## How to Contribute

Want to help implement these features?

1. **Check the issues** for features marked "help wanted"
2. **Comment on issues** to claim them
3. **Submit PRs** following the contribution guidelines
4. **Test and provide feedback** on new features
5. **Share your keymaps** with the community

---

## Release Schedule

- **Minor versions** (1.x.0): Every 3 months
- **Patch versions** (1.0.x): As needed for bug fixes
- **Major versions** (x.0.0): Annually or for breaking changes

---

## Feedback

This roadmap is a living document. If you have suggestions:

- Open a feature request issue
- Comment on existing issues
- Start a discussion on GitHub Discussions
- Vote on features you want

**Last Updated:** December 3, 2025
