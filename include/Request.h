#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @file Request.h
 * @brief Defines the Request struct used throughout the simulation.
 */

/**
 * @struct Request
 * @brief Represents a single incoming web request.
 */
struct Request {
    std::string ipIn;   ///< Source IP (client)
    std::string ipOut;  ///< Destination IP (server result)
    int time;           ///< Processing time (clock cycles)
    char jobType;       ///< 'P' = processing, 'S' = streaming
    long id;            ///< unique id for logging

    /**
     * @brief Default constructor initializing an empty/dummy request.
     */
    Request();

    /**
     * @brief Parameterized constructor to fully define a simulated request.
     * @param in The client Source IP.
     * @param out The server Destination IP.
     * @param t The required processing time in clock cycles.
     * @param jt The type of job ('P' for processing, 'S' for streaming).
     * @param id_ The unique identifier for this request.
     */
    Request(const std::string &in, const std::string &out, int t, char jt, long id_);
};

#endif