/**
 * @file Logger.cpp
 * @brief Implementation of the Logger class providing thread-safe file logging capabilities.
 */

#include "Logger.h"
#include <iostream>

/**
 * @brief Destructor for the Logger class.
 * Ensures that the underlying file stream is safely closed upon destruction.
 */
Logger::~Logger() { close(); }

/**
 * @brief Opens a file stream for logging.
 * @param filename The path and name of the file to open.
 * @return true if the file was successfully opened, false otherwise.
 */
bool Logger::open(const std::string &filename) {
    ofs.open(filename);
    return ofs.is_open();
}

/**
 * @brief Safely closes the active log file stream if it is currently open.
 */
void Logger::close() {
    if (ofs.is_open()) {
        ofs.close();
    }
}

/**
 * @brief Writes a string message to the log file in a thread-safe manner.
 * Uses a mutex lock to ensure that concurrent log calls do not interleave data.
 * @param s The message string to be logged.
 */
void Logger::log(const std::string &s) {
    std::lock_guard<std::mutex> lk(m);
    if (ofs.is_open()) {
        ofs << s << std::endl;
    }
}