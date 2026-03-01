#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @file Utils.h
 * @brief Small utilities: random ip and random int.
 */

/**
 * @brief Generates a randomized standard IPv4 address string.
 * @return A string formatted as "X.X.X.X".
 */
std::string randomIp();

/**
 * @brief Generates a random integer within a specified inclusive range.
 * @param lo The lower bound (inclusive).
 * @param hi The upper bound (inclusive).
 * @return A random integer between lo and hi.
 */
int randBetween(int lo, int hi);

#endif