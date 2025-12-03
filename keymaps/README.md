# Keymaps

This directory contains keymap configurations for the ESP32-S3 Macropad. Keymaps define what each key does in different profiles.

## Available Keymaps

### default.json
The default keymap includes two profiles:
- **DEV**: Developer shortcuts for coding (Cut, Copy, Paste, VS Code commands)
- **MEETING**: Video conferencing controls (Mute, Video, Hand Raise, etc.)

## Keymap Structure

Each keymap JSON file has the following structure:

```json
{
  "name": "keymap-name",
  "version": "1.0.0",
  "description": "Description of the keymap",
  "author": "your-name",
  "layout": "4x4",
  "profiles": {
    "PROFILE_NAME": {
      "name": "Display Name",
      "description": "Profile description",
      "color": "#HEX_COLOR",
      "keys": {
        "key_position": {
          "label": "Short Label",
          "description": "Full description",
          "action": "key|consumer|system",
          "modifiers": ["ctrl", "shift", "alt", "gui"],
          "key": "character or key name",
          "hid_modifier": "0xHEX",
          "hid_keycode": "0xHEX"
        }
      }
    }
  },
  "encoder": {
    "modes": {
      "NORMAL": {
        "rotate_cw": {...},
        "rotate_ccw": {...},
        "short_press": {...},
        "long_press": {...}
      }
    }
  }
}
```

## Key Positions

The 4×4 keypad uses the following key positions:

```
┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ A │
├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ B │
├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ C │
├───┼───┼───┼───┤
│ * │ 0 │ # │ D │
└───┴───┴───┴───┘
```

## Action Types

### key
Standard keyboard key press with optional modifiers.

```json
{
  "action": "key",
  "modifiers": ["ctrl"],
  "key": "c",
  "hid_modifier": "0x01",
  "hid_keycode": "0x06"
}
```

### consumer
Consumer control actions (volume, media playback, etc.).

```json
{
  "action": "consumer",
  "key": "volume_up",
  "hid_usage": "0xE9"
}
```

### system
Internal system actions (profile switching, menu navigation).

```json
{
  "action": "system",
  "key": "profile_next"
}
```

## Modifiers

Available keyboard modifiers:

| Modifier | HID Code | Description        |
|----------|----------|--------------------|
| ctrl     | 0x01     | Left Control       |
| shift    | 0x02     | Left Shift         |
| alt      | 0x04     | Left Alt           |
| gui      | 0x08     | Left GUI (Win/Cmd) |

You can combine modifiers: `["ctrl", "shift"]` = `0x03`

## HID Keycodes Reference

Common HID keyboard usage codes:

| Key | HID Code | Key | HID Code |
|-----|----------|-----|----------|
| A-Z | 0x04-0x1D | F1-F12 | 0x3A-0x45 |
| 1-9 | 0x1E-0x26 | Enter | 0x28 |
| 0   | 0x27     | Esc   | 0x29 |
| Space | 0x2C   | Tab   | 0x2B |
| /   | 0x38     | `     | 0x35 |

See `default.json` for complete reference.

## Consumer Control Codes

Common consumer usage codes:

| Function | HID Code |
|----------|----------|
| Volume Up | 0xE9 |
| Volume Down | 0xEA |
| Mute | 0xE2 |
| Play/Pause | 0xCD |
| Next Track | 0xB5 |
| Previous Track | 0xB6 |

## Creating Custom Keymaps

1. Copy `default.json` to a new file (e.g., `my-keymap.json`)
2. Update the `name`, `version`, `description`, and `author` fields
3. Modify profiles and key mappings as needed
4. Add or remove profiles
5. Customize encoder behavior

### Example: Adding a Gaming Profile

```json
"GAMING": {
  "name": "Gaming Mode",
  "description": "Gaming shortcuts and macros",
  "color": "#FF0000",
  "keys": {
    "1": {
      "label": "Ability 1",
      "description": "Use first ability",
      "action": "key",
      "modifiers": [],
      "key": "q",
      "hid_modifier": "0x00",
      "hid_keycode": "0x14"
    },
    "2": {
      "label": "Ability 2",
      "description": "Use second ability",
      "action": "key",
      "modifiers": [],
      "key": "e",
      "hid_modifier": "0x00",
      "hid_keycode": "0x08"
    }
  }
}
```

## Application-Specific Shortcuts

### Microsoft Teams
- Mute/Unmute: Ctrl+Shift+M
- Video Toggle: Ctrl+Shift+O
- Hand Raise: Ctrl+Shift+K
- Share Screen: Ctrl+Shift+E

### Zoom
- Mute/Unmute: Alt+A (or Ctrl+Shift+M)
- Video Toggle: Alt+V
- Share Screen: Alt+S
- Record: Alt+R

### VS Code
- Command Palette: Ctrl+Shift+P
- Quick Open: Ctrl+P
- Terminal: Ctrl+`
- Comment Line: Ctrl+/
- Format: Ctrl+Shift+F

### Google Chrome
- New Tab: Ctrl+T
- Close Tab: Ctrl+W
- Reopen Tab: Ctrl+Shift+T
- Developer Tools: F12

## Testing Your Keymap

After creating a custom keymap:

1. Update the firmware to load your keymap
2. Upload to ESP32-S3
3. Test each key in the target application
4. Verify modifier combinations work correctly
5. Check encoder behavior

## Sharing Keymaps

If you create a useful keymap:

1. Add a descriptive filename (e.g., `streaming.json`, `productivity.json`)
2. Include complete metadata (name, version, description, author)
3. Add application compatibility notes in key descriptions
4. Submit via pull request to share with community

## Tips

- **Label keys clearly**: Use short, recognizable labels
- **Add descriptions**: Explain what each key does
- **Test thoroughly**: Verify all shortcuts in target applications
- **Document compatibility**: Note which apps each profile works with
- **Consider ergonomics**: Place frequently used keys in easy-to-reach positions
- **Group related functions**: Keep similar actions near each other

## Troubleshooting

### Key doesn't work
- Verify HID keycode is correct
- Check modifier combination
- Ensure target app supports the shortcut

### Wrong character sent
- Double-check HID keycode in reference table
- Verify keyboard layout matches (US, UK, etc.)

### Modifier not working
- Confirm modifier bitmask is correct
- Check if multiple modifiers need combining (OR operation)

## Resources

- [USB HID Usage Tables](https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf)
- [HID Keyboard Scan Codes](https://www.freebsddiary.org/APC/usb_hid_usages.php)
- Application keyboard shortcuts documentation

## License

Keymaps are community contributions and are available under the project license.
