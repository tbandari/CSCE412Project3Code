/**
 * @file Firewall.cpp
 * @brief Implementation of the Firewall class for IP prefix blocking.
 */

#include "Firewall.h"
#include <algorithm>

/**
 * @brief Adds a blocked IP prefix to the firewall rules.
 * * @param prefix The string prefix to block (e.g., "192.168.").
 */
void Firewall::addBlockedPrefix(const std::string &prefix) {
    prefixes.push_back(prefix);
}

/**
 * @brief Checks if a given IP address matches any blocked prefix.
 * * @param ip The IP address string to check against the active rules.
 * @return true if the IP starts with a blocked prefix, false otherwise.
 */
bool Firewall::isBlocked(const std::string &ip) const {
    for (const auto &p : prefixes) {
        if (ip.rfind(p, 0) == 0){
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns the total number of blocked prefixes currently active.
 * * @return The size of the active blocked prefixes list.
 */
size_t Firewall::blockedCount() const { 
    return prefixes.size(); 
}
