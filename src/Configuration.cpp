/**
 * @file Configuration.cpp
 * @brief Implementation of the Configuration class for parsing simple key-value property files.
 */

#include "Configuration.h"
#include <fstream>
#include <sstream>

/**
 * @brief Loads and parses key-value pairs from the specified file path.
 * * Reads the file line by line, ignores comments starting with '#', and extracts
 * key-value pairs separated by '='. Populates the internal map with these pairs.
 * * @param path The filesystem path to the config file to be loaded.
 * @return true if the file was successfully opened and parsed, false otherwise.
 */
bool Configuration::loadFromFile(const std::string &path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        auto comment = line.find('#');
        if (comment != std::string::npos){
            line = line.substr(0, comment);
        }
        std::istringstream iss(line);
        std::string kv;
        if (!(iss >> kv)){ 
            continue;
        }
        auto eq = kv.find('=');
        if (eq == std::string::npos){
            continue;
        }
        std::string k = kv.substr(0, eq);
        std::string v = kv.substr(eq+1);
        values[k] = v;
    }
    return true;
}

/**
 * @brief Retrieves a string configuration value.
 * * @param key The configuration key to look up.
 * @param def The default value to return if the key is not found in the configuration.
 * @return The string value associated with the key, or the provided default value if not found.
 */
std::string Configuration::getString(const std::string &key, const std::string &def) const {
    auto it = values.find(key);
    if (it == values.end()) {
        return def;
    }
    return it->second;
}

/**
 * @brief Retrieves an integer configuration value.
 * * Attempts to parse the string value associated with the given key into an integer.
 * If the key does not exist or the string cannot be parsed as an integer, the default
 * value is returned.
 * * @param key The configuration key to look up.
 * @param def The default integer value to return if the key is not found or parsing fails.
 * @return The integer value associated with the key, or the provided default value.
 */
int Configuration::getInt(const std::string &key, int def) const {
    auto s = getString(key, "");
    if (s.empty()) {
        return def;
    }
    try { 
        return std::stoi(s); 
    } 
    catch(...) { 
        return def; 
    }
}
