# Contributing to ESP32-S3 Macropad

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Documentation](#documentation)

## Code of Conduct

This project follows a Code of Conduct that all contributors are expected to adhere to. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/install)
- [Git](https://git-scm.com/)
- ESP32-S3 development board
- Basic understanding of C++ and Arduino framework

### Setup Development Environment

1. Fork the repository
2. Clone your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/ESP32-S3-Macropad.git
   cd ESP32-S3-Macropad
   ```

3. Add upstream remote:
   ```bash
   git remote add upstream https://github.com/tarak4v/ESP32-S3-Macropad.git
   ```

4. Install dependencies:
   ```bash
   pio pkg install
   ```

5. Build the project:
   ```bash
   pio run
   ```

## Development Workflow

1. **Create a branch** for your feature or fix:
   ```bash
   git checkout -b feature/your-feature-name
   ```
   or
   ```bash
   git checkout -b fix/issue-number-description
   ```

2. **Make your changes** following the coding standards

3. **Test thoroughly** on actual hardware

4. **Commit your changes** with clear messages

5. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Create a Pull Request** to the main repository

## Coding Standards

### C++ Style Guide

- **Indentation**: 2 spaces (no tabs)
- **Line Length**: Maximum 100 characters
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `DisplayManager`)
  - Functions: `camelCase` (e.g., `updateDisplay()`)
  - Variables: `camelCase` (e.g., `currentProfile`)
  - Constants: `UPPER_CASE` (e.g., `MAX_PROFILES`)
  - Private members: `_camelCase` (e.g., `_internalState`)

### Code Organization

```cpp
// 1. License header
// 2. File description
// 3. Include guards (for headers)
// 4. Includes (system, then library, then local)
// 5. Defines and constants
// 6. Type definitions
// 7. Global variables (avoid if possible)
// 8. Function declarations
// 9. Function implementations
```

### Example Header File

```cpp
/**
 * @file display_manager.h
 * @brief Display management for OLED screen
 * @author Your Name
 * @date 2025-12-03
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void update();
  void showProfile(const char* name);

private:
  Adafruit_SSD1306 _display;
  void _clearScreen();
};

#endif // DISPLAY_MANAGER_H
```

### Comments

- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Add doc comments for public functions:
  ```cpp
  /**
   * @brief Updates the display with current state
   * @param force Force full redraw if true
   * @return true if update successful
   */
  bool updateDisplay(bool force = false);
  ```

### Error Handling

- Check return values
- Use meaningful error messages
- Log errors to Serial when appropriate
- Don't crash the device - handle errors gracefully

### Memory Management

- Avoid dynamic allocation where possible
- Use `const` for read-only data
- Be mindful of stack vs heap usage
- Clean up resources in destructors

## Commit Guidelines

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, missing semicolons, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks
- `perf`: Performance improvements

### Examples

```
feat(keypad): add debounce configuration

Add configurable debounce time for keypad to reduce false triggers
in noisy environments.

Closes #123
```

```
fix(display): correct text alignment in menu

Fixed text overflow issue when profile names are too long.
Now truncates with ellipsis.
```

## Pull Request Process

1. **Update documentation** if needed
2. **Update CHANGELOG.md** with your changes
3. **Ensure CI passes** (all builds successful)
4. **Request review** from maintainers
5. **Address feedback** promptly
6. **Squash commits** if requested
7. **Wait for approval** before merging

### PR Title Format

Follow commit message conventions:
```
feat: add customizable LED colors
fix: resolve encoder direction bug
docs: update wiring diagram
```

### PR Description Template

```markdown
## Description
Brief description of changes

## Motivation and Context
Why is this change needed? What problem does it solve?

## Testing
- [ ] Tested on ESP32-S3 hardware
- [ ] No compilation warnings
- [ ] Documentation updated

## Screenshots (if applicable)
Add screenshots or videos demonstrating the changes

## Related Issues
Fixes #123
Related to #456
```

## Testing

### Before Submitting

- [ ] Code compiles without errors or warnings
- [ ] Tested on actual ESP32-S3 hardware
- [ ] All existing features still work
- [ ] New features have been tested thoroughly
- [ ] Documentation updated
- [ ] No memory leaks
- [ ] Performance acceptable

### Hardware Testing Checklist

- [ ] OLED display updates correctly
- [ ] All 16 keypad buttons work
- [ ] Encoder rotation detected
- [ ] Encoder button press works
- [ ] Long press detection works
- [ ] Profile switching functions
- [ ] HID keys send correctly
- [ ] Volume control works
- [ ] LED indicators display correct status

### Simulation Testing

If adding Wokwi support:
- [ ] Circuit diagram is correct
- [ ] Simulation runs without errors
- [ ] Key functionality works in simulation

## Documentation

### What Needs Documentation

- New features
- API changes
- Configuration options
- Hardware changes
- Breaking changes

### Where to Document

- **Code comments**: For implementation details
- **Header files**: For API documentation
- **README.md**: For user-facing features
- **docs/**: For detailed technical documentation
- **keymaps/**: For keymap configurations
- **CHANGELOG.md**: For all changes

### Documentation Style

- Write in clear, concise English
- Use present tense ("adds feature" not "added feature")
- Include code examples where helpful
- Add diagrams for complex concepts
- Test all code examples

## Adding New Features

### New Hardware Components

1. Update `docs/wiring.md` with pin mappings
2. Add connection diagram
3. Update schematic
4. Document in README.md
5. Add to bill of materials

### New Keymaps

1. Create JSON file in `keymaps/`
2. Follow existing format
3. Document all shortcuts
4. Test with target applications
5. Update keymaps/README.md

### New Profiles

1. Add to `keymaps/default.json`
2. Document key mappings
3. Test all keys
4. Add usage examples

## Reporting Issues

### Bug Reports

Include:
- ESP32-S3 board version
- PlatformIO version
- Firmware version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Serial monitor output
- Photos/videos if applicable

### Feature Requests

Include:
- Clear description
- Use case
- Example implementation (optional)
- Mock-ups or diagrams (optional)

## Getting Help

- **Documentation**: Check README.md and docs/
- **Issues**: Search existing issues
- **Discussions**: Use GitHub Discussions for questions
- **Community**: Reach out to maintainers

## Recognition

Contributors will be:
- Listed in CHANGELOG.md
- Credited in release notes
- Added to contributors list
- Mentioned in relevant documentation

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to ESP32-S3 Macropad! 🎉
