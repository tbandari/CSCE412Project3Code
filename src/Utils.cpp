/**
 * @file Utils.cpp
 * @brief Implementation of utility functions for generating random IP addresses and integers.
 */

#include "Utils.h"
#include <random>
#include <sstream>

/**
 * @brief Standard process used for generating random numbers, seeded with the current time.
 */
static std::mt19937 rng((unsigned)time(nullptr));

/**
 * @brief Generates a randomized standard IPv4 address string.
 * * Creates four random integers between 0 and 255 and formats them 
 * into a standard dot-decimal notation IP address string.
 * * @return A string formatted as "X.X.X.X".
 */
std::string randomIp() {
    std::uniform_int_distribution<int> d(0,255);
    std::ostringstream ss;
    ss << d(rng) << '.' << d(rng) << '.' << d(rng) << '.' << d(rng);
    return ss.str();
}

/**
 * @brief Generates a random integer within a specified inclusive range.
 * * @param lo The lower bound of the desired range (inclusive).
 * @param hi The upper bound of the desired range (inclusive).
 * @return A random integer between lo and hi.
 */
int randBetween(int lo, int hi) {
    std::uniform_int_distribution<int> d(lo, hi);
    return d(rng);
}
