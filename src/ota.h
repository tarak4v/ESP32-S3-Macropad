/**
 * @file ota.h
 * @brief OTA (Over-The-Air) firmware update manager
 * @author tarak4v
 * @date 2025-12-05
 */

#ifndef OTA_H
#define OTA_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Config.h"

/**
 * @brief OTA update manager
 * 
 * Handles WiFi connection and OTA firmware updates.
 * Allows wireless firmware uploads via PlatformIO or Arduino IDE.
 */
class OTAManager {
public:
  /**
   * @brief Constructor
   */
  OTAManager();
  
  /**
   * @brief Initialize OTA with WiFi credentials
   * @param ssid WiFi network name
   * @param password WiFi password
   * @param hostname Device hostname (default: "esp32s3-macropad")
   * @param port OTA port (default: 3232)
   * @return true if initialization successful
   */
  bool begin(const char* ssid, const char* password, 
             const char* hostname = "esp32s3-macropad", 
             uint16_t port = 3232);
  
  /**
   * @brief Update OTA handler (call in loop)
   * 
   * Must be called frequently in main loop to handle OTA requests.
   */
  void handle();
  
  /**
   * @brief Check if WiFi is connected
   * @return true if connected
   */
  bool isConnected() const;
  
  /**
   * @brief Get WiFi RSSI (signal strength)
   * @return Signal strength in dBm
   */
  int getRSSI() const;
  
  /**
   * @brief Get local IP address
   * @return IP address as string
   */
  String getIPAddress() const;
  
  /**
   * @brief Get hostname
   * @return Hostname string
   */
  String getHostname() const;
  
  /**
   * @brief Enable/disable OTA
   * @param enable true to enable, false to disable
   */
  void setEnabled(bool enable);
  
  /**
   * @brief Check if OTA is enabled
   * @return true if enabled
   */
  bool isEnabled() const { return _enabled; }
  
  /**
   * @brief Check if OTA update is in progress
   * @return true if updating
   */
  bool isUpdating() const { return _updating; }

private:
  bool _initialized;
  bool _enabled;
  bool _updating;
  String _hostname;
  
  // Callback for OTA events
  static void onStart();
  static void onEnd();
  static void onProgress(unsigned int progress, unsigned int total);
  static void onError(ota_error_t error);
};

#endif // OTA_H
