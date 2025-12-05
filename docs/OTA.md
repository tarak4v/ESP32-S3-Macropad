# OTA (Over-The-Air) Update Guide

## Overview

OTA functionality allows you to upload new firmware wirelessly without connecting a USB cable. After the initial USB flash, all future updates can be done over WiFi.

## Initial Setup

### 1. Configure WiFi Credentials

Edit `include/Config.h` and update these lines:

```cpp
#define WIFI_SSID                 "YourWiFiSSID"
#define WIFI_PASSWORD             "YourWiFiPassword"
```

### 2. First Flash via USB

Flash the firmware with OTA support via USB:

```bash
pio run -t upload
```

### 3. Note the Device IP

After booting, the serial monitor will show:

```
✓ OTA Enabled
  Hostname: esp32s3-macropad.local
  IP: 192.168.1.100
  Port: 3232

Ready for wireless updates!
```

## OTA Updates

### Method 1: Using PlatformIO Command Line

```bash
# Upload via IP address
pio run -t upload --upload-port 192.168.1.100

# Upload via hostname (mDNS)
pio run -t upload --upload-port esp32s3-macropad.local
```

### Method 2: Configure platformio.ini

Uncomment and update in `platformio.ini`:

```ini
upload_protocol = espota
upload_port = 192.168.1.100  ; Your device IP
upload_flags = 
    --port=3232
```

Then simply run:
```bash
pio run -t upload
```

### Method 3: VS Code PlatformIO Extension

1. Click **PlatformIO: Upload** from the status bar
2. If multiple targets appear, select the OTA one
3. Or use Command Palette: `PlatformIO: Upload using Programmer`

## Features

✅ **Automatic WiFi Reconnection** - Reconnects if WiFi drops  
✅ **mDNS Support** - Access via `esp32s3-macropad.local`  
✅ **Progress Monitoring** - See upload progress in serial  
✅ **Error Handling** - Detailed error messages  
✅ **No USB Required** - Update from anywhere on network  

## Security

### Optional: Add OTA Password

In `src/ota.cpp`, uncomment:

```cpp
ArduinoOTA.setPassword("admin");
```

Then add to platformio.ini:

```ini
upload_flags = 
    --port=3232
    --auth=admin
```

## Troubleshooting

### Device Not Found

- Check device is powered on
- Verify device is on same WiFi network
- Try IP address instead of hostname
- Check firewall settings (port 3232)

### Upload Fails

- Ensure device isn't running other code that blocks OTA
- Try power cycling the device
- Check WiFi signal strength
- Verify credentials are correct

### WiFi Won't Connect

- Check SSID and password in Config.h
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Check router allows ESP32 connections

## Disabling OTA

Set in `include/Config.h`:

```cpp
#define OTA_ENABLED               false
```

This saves memory and disables WiFi if not needed.

## Technical Details

- **Protocol**: ESP OTA (Arduino OTA)
- **Port**: 3232 (default)
- **mDNS**: Enabled (`.local` domain)
- **WiFi Mode**: Station (STA)
- **Max Firmware Size**: ~3.3 MB (depends on partition scheme)

## Network Requirements

- 2.4GHz WiFi network
- Device and computer on same network
- UDP port 3232 open
- mDNS port 5353 (for hostname resolution)
