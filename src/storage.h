/**
 * @file storage.h
 * @brief Configuration storage and keymap loading
 * @author tarak4v
 * @date 2025-12-03
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "Types.h"

/**
 * @brief Storage manager for configuration and keymaps
 * 
 * Handles persistent storage using NVS (Preferences) for system
 * configuration and LittleFS for keymap JSON files.
 */
class StorageManager {
public:
  /**
   * @brief Constructor
   */
  StorageManager();
  
  /**
   * @brief Initialize storage systems
   * @return true if initialization successful
   */
  bool begin();
  
  /**
   * @brief Load system configuration from NVS
   * @param config Output parameter for configuration
   * @return true if config loaded successfully
   */
  bool loadConfig(SystemConfig& config);
  
  /**
   * @brief Save system configuration to NVS
   * @param config Configuration to save
   * @return true if saved successfully
   */
  bool saveConfig(const SystemConfig& config);
  
  /**
   * @brief Load keymap from JSON file
   * @param filename Keymap filename (e.g., "default.json")
   * @param profiles Output array for profiles
   * @param maxProfiles Maximum number of profiles to load
   * @return Number of profiles loaded (0 on error)
   */
  uint8_t loadKeymap(const char* filename, Profile* profiles, uint8_t maxProfiles);
  
  /**
   * @brief Save keymap to JSON file
   * @param filename Keymap filename
   * @param profiles Array of profiles to save
   * @param profileCount Number of profiles
   * @return true if saved successfully
   */
  bool saveKeymap(const char* filename, const Profile* profiles, uint8_t profileCount);
  
  /**
   * @brief List available keymap files
   * @param keymaps Output array for filenames
   * @param maxKeymaps Maximum number of keymaps to list
   * @return Number of keymaps found
   */
  uint8_t listKeymaps(char keymaps[][32], uint8_t maxKeymaps);
  
  /**
   * @brief Factory reset - clear all stored data
   * @return true if reset successful
   */
  bool factoryReset();
  
  /**
   * @brief Get filesystem usage info
   * @param totalBytes Output for total bytes
   * @param usedBytes Output for used bytes
   * @return true if info retrieved successfully
   */
  bool getStorageInfo(size_t& totalBytes, size_t& usedBytes);
  
  /**
   * @brief Check if storage is initialized
   * @return true if initialized
   */
  bool isInitialized() const { return _initialized; }

private:
  Preferences _prefs;
  bool _initialized;
  bool _littleFSMounted;
  
  // JSON parsing helpers
  bool parseProfile(JsonObject profileObj, Profile& profile);
  bool parseKeyAction(JsonObject keyObj, KeyAction& action);
  uint8_t parseHexKeycode(const char* hex);
};

#endif // STORAGE_H
