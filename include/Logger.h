#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

/**
 * @file Logger.h
 * @brief Simple thread-safe logger that writes to file and optionally to console.
 */

/**
 * @class Logger
 * @brief Thread-safe logging utility for writing simulation events to an output file.
 */
class Logger {
public:
    /**
     * @brief Default constructor.
     */
    Logger() = default;

    /**
     * @brief Destructor, ensures the file stream is safely closed.
     */
    ~Logger();

    /**
     * @brief Opens a file stream for logging.
     * @param filename The path and name of the file to open.
     * @return true if the file was successfully opened, false otherwise.
     */
    bool open(const std::string &filename);

    /**
     * @brief Safely closes the active log file stream.
     */
    void close();

    /**
     * @brief Writes a string message to the log file in a thread-safe manner.
     * @param s The message string to log.
     */
    void log(const std::string &s);

    /**
     * @brief Convenience method to print a colored log message directly to the console.
     * @param s The message string to output.
     * @param ansiColor The raw ANSI color code sequence to format the string.
     */
    void logConsole(const std::string &s, const std::string &ansiColor="");
    
private:
    std::ofstream ofs; ///< File output stream for writing log data
    std::mutex m;      ///< Mutex ensuring thread-safe logging operations
};

#endif