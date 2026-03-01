/**
 * @file Request.cpp
 * @brief Implementation of the Request struct representing a single simulated web request.
 */

#include "Request.h"

/**
 * @brief Default constructor for a Request.
 * Initializes a request with default placeholder IP addresses ("0.0.0.0"), 
 * zero processing time, a default job type of 'P' (processing), and an ID of 0.
 */
Request::Request(): ipIn("0.0.0.0"), ipOut("0.0.0.0"), time(0), jobType('P'), id(0) {}

/**
 * @brief Parameterized constructor to initialize a newly generated Request.
 * @param in The source (client) IP address originating the request.
 * @param out The destination (server) IP address responding to the request.
 * @param t The amount of time (in clock cycles) required to complete the request.
 * @param jt The job type identifier ('P' for processing, 'S' for streaming).
 * @param id_ A unique integer identifier for tracking and logging the request.
 */
Request::Request(const std::string &in, const std::string &out, int t, char jt, long id_): ipIn(in), ipOut(out), time(t), jobType(jt), id(id_) {}
