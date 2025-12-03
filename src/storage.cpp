/**
 * @file storage.cpp
 * @brief Configuration storage implementation
 * @author tarak4v
 * @date 2025-12-03
 */

#include "storage.h"

StorageManager::StorageManager() 
  : _initialized(false)
  , _littleFSMounted(false) {
}

bool StorageManager::begin() {
  DEBUG_PRINTLN("StorageManager: Initializing...");
  
  // Initialize LittleFS
  if (!LittleFS.begin(true)) {  // true = format on fail
    DEBUG_PRINTLN("StorageManager: LittleFS mount failed");
    _littleFSMounted = false;
  } else {
    _littleFSMounted = true;
    DEBUG_PRINTLN("StorageManager: LittleFS mounted");
  }
  
  // Initialize NVS
  if (!_prefs.begin(CONFIG_NAMESPACE, false)) {
    DEBUG_PRINTLN("StorageManager: NVS initialization failed");
    return false;
  }
  
  _initialized = true;
  
  // Log storage info
  size_t total, used;
  if (getStorageInfo(total, used)) {
    DEBUG_PRINTF("StorageManager: %zu / %zu bytes used\n", used, total);
  }
  
  DEBUG_PRINTLN("StorageManager: Initialized successfully");
  return true;
}

bool StorageManager::loadConfig(SystemConfig& config) {
  if (!_initialized) {
    DEBUG_PRINTLN("StorageManager: Not initialized");
    return false;
  }
  
  #if DEBUG_CONFIG
  DEBUG_PRINTLN("StorageManager: Loading config...");
  #endif
  
  // Load values with defaults
  config.currentProfile = _prefs.getUChar("profile", DefaultConfig::profile);
  config.brightness = _prefs.getUChar("brightness", DefaultConfig::brightness);
  config.debounceTime = _prefs.getUChar("debounce", DefaultConfig::debounce);
  config.longPressTime = _prefs.getUShort("longPress", DefaultConfig::longPress);
  
  String keymapName = _prefs.getString("keymap", DefaultConfig::keymap);
  strncpy(config.keymapName, keymapName.c_str(), sizeof(config.keymapName) - 1);
  config.keymapName[sizeof(config.keymapName) - 1] = '\0';
  
  #if DEBUG_CONFIG
  DEBUG_PRINTF("  Profile: %d\n", config.currentProfile);
  DEBUG_PRINTF("  Brightness: %d\n", config.brightness);
  DEBUG_PRINTF("  Keymap: %s\n", config.keymapName);
  #endif
  
  return true;
}

bool StorageManager::saveConfig(const SystemConfig& config) {
  if (!_initialized) {
    return false;
  }
  
  #if DEBUG_CONFIG
  DEBUG_PRINTLN("StorageManager: Saving config...");
  #endif
  
  _prefs.putUChar("profile", config.currentProfile);
  _prefs.putUChar("brightness", config.brightness);
  _prefs.putUChar("debounce", config.debounceTime);
  _prefs.putUShort("longPress", config.longPressTime);
  _prefs.putString("keymap", config.keymapName);
  
  #if DEBUG_CONFIG
  DEBUG_PRINTLN("  Config saved");
  #endif
  
  return true;
}

uint8_t StorageManager::loadKeymap(const char* filename, Profile* profiles, uint8_t maxProfiles) {
  if (!_initialized || !_littleFSMounted) {
    DEBUG_PRINTLN("StorageManager: Cannot load keymap - storage not ready");
    return 0;
  }
  
  // Build full path
  String path = "/keymaps/";
  path += filename;
  
  DEBUG_PRINTF("StorageManager: Loading keymap from %s\n", path.c_str());
  
  // Check if file exists
  if (!LittleFS.exists(path)) {
    DEBUG_PRINTF("  File not found: %s\n", path.c_str());
    return 0;
  }
  
  // Open file
  File file = LittleFS.open(path, "r");
  if (!file) {
    DEBUG_PRINTLN("  Failed to open file");
    return 0;
  }
  
  // Parse JSON
  JsonDocument doc;  // Dynamic allocation
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  
  if (error) {
    DEBUG_PRINTF("  JSON parse error: %s\n", error.c_str());
    return 0;
  }
  
  // Extract profiles
  JsonArray profilesArray = doc["profiles"];
  if (!profilesArray) {
    DEBUG_PRINTLN("  No 'profiles' array found");
    return 0;
  }
  
  uint8_t count = 0;
  for (JsonObject profileObj : profilesArray) {
    if (count >= maxProfiles) break;
    
    if (parseProfile(profileObj, profiles[count])) {
      count++;
    }
  }
  
  DEBUG_PRINTF("  Loaded %d profiles\n", count);
  return count;
}

bool StorageManager::saveKeymap(const char* filename, const Profile* profiles, uint8_t profileCount) {
  if (!_initialized || !_littleFSMounted) {
    return false;
  }
  
  // Build full path
  String path = "/keymaps/";
  path += filename;
  
  DEBUG_PRINTF("StorageManager: Saving keymap to %s\n", path.c_str());
  
  // Create JSON document
  JsonDocument doc;
  JsonArray profilesArray = doc["profiles"].to<JsonArray>();
  
  // Serialize profiles (simplified - full implementation would include all fields)
  for (uint8_t i = 0; i < profileCount; i++) {
    JsonObject profileObj = profilesArray.add<JsonObject>();
    profileObj["name"] = profiles[i].name;
    // TODO: Add keys serialization
  }
  
  // Open file for writing
  File file = LittleFS.open(path, "w");
  if (!file) {
    DEBUG_PRINTLN("  Failed to create file");
    return false;
  }
  
  // Write JSON
  serializeJson(doc, file);
  file.close();
  
  DEBUG_PRINTLN("  Keymap saved");
  return true;
}

uint8_t StorageManager::listKeymaps(char keymaps[][32], uint8_t maxKeymaps) {
  if (!_initialized || !_littleFSMounted) {
    return 0;
  }
  
  File root = LittleFS.open("/keymaps");
  if (!root || !root.isDirectory()) {
    return 0;
  }
  
  uint8_t count = 0;
  File file = root.openNextFile();
  while (file && count < maxKeymaps) {
    if (!file.isDirectory()) {
      String name = file.name();
      if (name.endsWith(".json")) {
        strncpy(keymaps[count], name.c_str(), 31);
        keymaps[count][31] = '\0';
        count++;
      }
    }
    file = root.openNextFile();
  }
  
  return count;
}

bool StorageManager::factoryReset() {
  if (!_initialized) {
    return false;
  }
  
  DEBUG_PRINTLN("StorageManager: Factory reset");
  
  // Clear NVS
  _prefs.clear();
  
  // Format LittleFS (optional - uncomment if needed)
  // LittleFS.format();
  
  return true;
}

bool StorageManager::getStorageInfo(size_t& totalBytes, size_t& usedBytes) {
  if (!_littleFSMounted) {
    return false;
  }
  
  totalBytes = LittleFS.totalBytes();
  usedBytes = LittleFS.usedBytes();
  return true;
}

// Private helper functions

bool StorageManager::parseProfile(JsonObject profileObj, Profile& profile) {
  // Parse profile name
  const char* name = profileObj["name"];
  if (!name) {
    DEBUG_PRINTLN("  Profile missing 'name' field");
    return false;
  }
  
  strncpy(profile.name, name, sizeof(profile.name) - 1);
  profile.name[sizeof(profile.name) - 1] = '\0';
  
  // Parse color (optional)
  if (profileObj["color"].is<uint32_t>()) {
    profile.color = profileObj["color"].as<uint32_t>();
  }
  
  // Parse keys array
  JsonArray keysArray = profileObj["keys"];
  if (!keysArray) {
    DEBUG_PRINTLN("  Profile missing 'keys' array");
    return false;
  }
  
  int keyIndex = 0;
  for (JsonObject keyObj : keysArray) {
    if (keyIndex >= 16) break;
    
    if (parseKeyAction(keyObj, profile.keys[keyIndex])) {
      keyIndex++;
    }
  }
  
  return true;
}

bool StorageManager::parseKeyAction(JsonObject keyObj, KeyAction& action) {
  // Check for consumer control (media key)
  if (keyObj["consumer"].is<const char*>()) {
    const char* consumer = keyObj["consumer"];
    action.consumer = parseHexKeycode(consumer);
    action.modifiers = 0;
    action.keycode = 0;
    return true;
  }
  
  // Parse keyboard action
  if (keyObj["key"].is<const char*>()) {
    const char* key = keyObj["key"];
    action.keycode = parseHexKeycode(key);
    
    // Parse modifiers (optional)
    action.modifiers = 0;
    if (keyObj["modifiers"].is<JsonArray>()) {
      JsonArray mods = keyObj["modifiers"];
      for (JsonVariant mod : mods) {
        const char* modStr = mod.as<const char*>();
        if (strcmp(modStr, "CTRL") == 0) action.modifiers |= MOD_LCTRL;
        else if (strcmp(modStr, "SHIFT") == 0) action.modifiers |= MOD_LSHIFT;
        else if (strcmp(modStr, "ALT") == 0) action.modifiers |= MOD_LALT;
        else if (strcmp(modStr, "GUI") == 0) action.modifiers |= MOD_LGUI;
      }
    }
    
    action.consumer = 0;
    return true;
  }
  
  return false;
}

uint8_t StorageManager::parseHexKeycode(const char* hex) {
  if (!hex) return 0;
  
  // Skip "0x" prefix if present
  if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
    hex += 2;
  }
  
  return (uint8_t)strtol(hex, nullptr, 16);
}
