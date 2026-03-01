#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>

/**
 * @file Configuration.h
 * @brief Tiny parser for key=value config files.
 *
 * Example (config.cfg):
 * numServers=10
 * simulationTime=10000
 * minQueueFactor=50
 * maxQueueFactor=80
 * restCycles=10
 * seed=42
 * initialQueuePerServer=100
 * arrivalRate=2
 * blockedPrefix=192.168.
 */

/**
 * @class Configuration
 * @brief Parses and stores configuration key-value pairs from a text file.
 */
class Configuration {
public:
    /**
     * @brief Default constructor.
     */
    Configuration() = default;

    /**
     * @brief Loads and parses key-value pairs from the specified file path.
     * @param path The filesystem path to the config file.
     * @return true if the file was loaded and parsed successfully, false otherwise.
     */
    bool loadFromFile(const std::string &path);

    /**
     * @brief Retrieves a string configuration value.
     * @param key The configuration key to look up.
     * @param def The default value to return if the key is not found.
     * @return The string value associated with the key, or the default value.
     */
    std::string getString(const std::string &key, const std::string &def="") const;

    /**
     * @brief Retrieves an integer configuration value.
     * @param key The configuration key to look up.
     * @param def The default integer value to return if the key is not found.
     * @return The integer value associated with the key, or the default value.
     */
    int getInt(const std::string &key, int def) const;

private:
    std::map<std::string, std::string> values; ///< Map storing the raw key-value pairs
};

#endif