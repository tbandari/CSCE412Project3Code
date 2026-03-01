#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @file WebServer.h
 * @brief Represents a single web server that can process a Request.
 */

/**
 * @class WebServer
 * @brief A node that handles the clock-cycle processing of individual assigned requests.
 */
class WebServer {
public:
    /**
     * @brief Constructs a WebServer with a specific identifier.
     * @param id The unique integer ID for this server.
     */
    WebServer(int id);

    /**
     * @brief Default destructor.
     */
    ~WebServer() = default;

    /**
     * @brief Assigns a request to the server, marks it busy, and sets its time remaining.
     * @param r The Request object to process.
     */
    void assignRequest(const Request &r);

    /**
     * @brief Simulates one clock cycle by decrementing the request's time remaining.
     * @return true if the request completed during this cycle, false otherwise.
     */
    bool processOneCycle();

    /**
     * @brief Checks if the server is currently available to take a new request.
     * @return true if the server is idle, false if it is busy.
     */
    bool isIdle() const;

    /**
     * @brief Retrieves the server's unique identifier.
     * @return The server ID.
     */
    int getId() const;

    /**
     * @brief Retrieves the remaining clock cycles needed to finish the active request.
     * @return The remaining processing time.
     */
    int getTimeRemaining() const;

private:
    int serverId;           ///< Unique identifier for the server
    bool busy;              ///< State flag indicating if the server has an active request
    Request currentRequest; ///< The current request being processed
    int timeRemaining;      ///< Clock cycles left to complete the current request
};

#endif