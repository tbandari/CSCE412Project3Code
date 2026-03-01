#ifndef FIREWALL_H
#define FIREWALL_H

#include <vector>
#include <string>

/**
 * @file Firewall.h
 * @brief Simple IP range blocker.
 *
 * The firewall holds a list of string prefixes that, if matched at the
 * beginning of an IP string, mark it blocked. Examples:
 * - "192.168." blocks 192.168.x.x
 * - "10." blocks 10.x.x.x
 */

/**
 * @class Firewall
 * @brief Blocks incoming requests based on a configured list of IP prefixes.
 */
class Firewall {
public:
    /**
     * @brief Default constructor.
     */
    Firewall() = default;

    /**
     * @brief Adds a blocked IP prefix to the firewall rules.
     * @param prefix The string prefix to block (e.g., "192.168.").
     */
    void addBlockedPrefix(const std::string &prefix);

    /**
     * @brief Checks if a given IP address matches any blocked prefix.
     * @param ip The IP address string to check.
     * @return true if the IP matches a blocked prefix, false otherwise.
     */
    bool isBlocked(const std::string &ip) const;

    /**
     * @brief Returns the total number of blocked prefixes currently active.
     * @return The size of the active prefixes list.
     */
    size_t blockedCount() const;

private:
    std::vector<std::string> prefixes; ///< List of active IP prefixes designated for blocking
};

#endif