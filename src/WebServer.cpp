/**
 * @file WebServer.cpp
 * @brief Implementation of the WebServer class representing a single processing node.
 */

#include "WebServer.h"

/**
 * @brief Constructs a WebServer with a given ID and initializes it to an idle state.
 * @param id The unique integer identifier for this server.
 */
WebServer::WebServer(int id) : serverId(id), busy(false), currentRequest(), timeRemaining(0) {}

/**
 * @brief Assigns a new request to the server to begin processing.
 * Updates the server's state to busy and sets the countdown for the required processing time.
 * @param r The Request object to be processed.
 */
void WebServer::assignRequest(const Request &r) {
    currentRequest = r;
    timeRemaining = r.time;
    busy = true;
}

/**
 * @brief Simulates a single clock cycle of processing.
 * Decrements the remaining time for the current request if the server is busy.
 * @return true if the assigned request finished during this cycle, false otherwise.
 */
bool WebServer::processOneCycle() {
    if (!busy) {
        return false;
    }
    --timeRemaining;
    if (timeRemaining <= 0) {
        busy = false;
        return true;
    }
    return false;
}

/**
 * @brief Checks if the server is currently idle and available to take a new request.
 * @return true if the server is not processing a request (idle), false if it is busy.
 */
bool WebServer::isIdle() const {
    return !busy;
}

/**
 * @brief Retrieves the server's unique identifier.
 * @return The integer ID of the server.
 */
int WebServer::getId() const { 
    return serverId; 
}

/**
 * @brief Retrieves the number of clock cycles remaining to finish the current request.
 * @return The remaining processing time in clock cycles.
 */
int WebServer::getTimeRemaining() const { 
    return timeRemaining; 
}
