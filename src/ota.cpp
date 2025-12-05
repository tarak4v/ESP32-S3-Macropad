/**
 * @file ota.cpp
 * @brief OTA firmware update implementation
 * @author tarak4v
 * @date 2025-12-05
 */

#include "ota.h"

OTAManager::OTAManager() 
  : _initialized(false)
  , _enabled(true)
  , _updating(false) {
}

bool OTAManager::begin(const char* ssid, const char* password, 
                       const char* hostname, uint16_t port) {
  DEBUG_PRINTLN("OTAManager: Initializing...");
  
  _hostname = hostname;
  
  // Connect to WiFi
  DEBUG_PRINTF("Connecting to WiFi: %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Wait for connection (max 20 seconds)
  int timeout = 40;  // 40 * 500ms = 20 seconds
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    DEBUG_PRINT(".");
    timeout--;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    DEBUG_PRINTLN("\nOTAManager: WiFi connection failed");
    return false;
  }
  
  DEBUG_PRINTLN("\nOTAManager: WiFi connected");
  DEBUG_PRINTF("  IP: %s\n", WiFi.localIP().toString().c_str());
  DEBUG_PRINTF("  Hostname: %s\n", hostname);
  
  // Configure OTA
  ArduinoOTA.setPort(port);
  ArduinoOTA.setHostname(hostname);
  
  // Optional: Set password for OTA
  // ArduinoOTA.setPassword("admin");
  
  // Configure callbacks
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    DEBUG_PRINTF("OTA: Start updating %s\n", type.c_str());
  });
  
  ArduinoOTA.onEnd([]() {
    DEBUG_PRINTLN("\nOTA: Update complete");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    static unsigned int lastPercent = 0;
    unsigned int percent = (progress / (total / 100));
    if (percent != lastPercent && percent % 10 == 0) {
      DEBUG_PRINTF("OTA Progress: %u%%\n", percent);
      lastPercent = percent;
    }
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG_PRINTF("OTA Error[%u]: ", error);
    switch (error) {
      case OTA_AUTH_ERROR:    DEBUG_PRINTLN("Auth Failed"); break;
      case OTA_BEGIN_ERROR:   DEBUG_PRINTLN("Begin Failed"); break;
      case OTA_CONNECT_ERROR: DEBUG_PRINTLN("Connect Failed"); break;
      case OTA_RECEIVE_ERROR: DEBUG_PRINTLN("Receive Failed"); break;
      case OTA_END_ERROR:     DEBUG_PRINTLN("End Failed"); break;
      default:                DEBUG_PRINTLN("Unknown Error"); break;
    }
  });
  
  ArduinoOTA.begin();
  
  // Start mDNS
  if (MDNS.begin(hostname)) {
    DEBUG_PRINTF("OTAManager: mDNS started: %s.local\n", hostname);
    MDNS.addService("arduino", "tcp", port);
  }
  
  _initialized = true;
  DEBUG_PRINTLN("OTAManager: Initialized successfully");
  
  return true;
}

void OTAManager::handle() {
  if (!_initialized || !_enabled) {
    return;
  }
  
  // Handle OTA requests
  ArduinoOTA.handle();
  
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long lastReconnect = 0;
    if (millis() - lastReconnect > 30000) {  // Try reconnect every 30s
      DEBUG_PRINTLN("OTAManager: WiFi disconnected, reconnecting...");
      WiFi.reconnect();
      lastReconnect = millis();
    }
  }
}

bool OTAManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

int OTAManager::getRSSI() const {
  return WiFi.RSSI();
}

String OTAManager::getIPAddress() const {
  return WiFi.localIP().toString();
}

String OTAManager::getHostname() const {
  return _hostname;
}

void OTAManager::setEnabled(bool enable) {
  _enabled = enable;
  DEBUG_PRINTF("OTAManager: %s\n", enable ? "Enabled" : "Disabled");
}
