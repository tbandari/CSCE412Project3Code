#ifndef JOBSWITCH_H
#define JOBSWITCH_H

#include "Request.h"
#include "LoadBalancer.h"
#include "Logger.h"

/**
 * @file JobSwitch.h
 * @brief Routes incoming requests to different LoadBalancer(s) based on jobType.
 */

/**
 * @class JobSwitch
 * @brief Handles routing of incoming requests to specialized load balancers.
 */
class JobSwitch {
public:
    /**
     * @brief Constructs a JobSwitch with streaming and processing load balancers.
     * @param streamLB Pointer to the load balancer handling streaming requests.
     * @param procLB Pointer to the load balancer handling processing requests.
     * @param logger Reference to the shared logging utility.
     */
    JobSwitch(LoadBalancer *streamLB, LoadBalancer *procLB, Logger &logger);

    /**
     * @brief Evaluates the request's job type and routes it to the correct load balancer.
     * @param r The request to be routed.
     */
    void routeRequest(const Request &r);

    /**
     * @brief Prints a combined simulation summary for all managed load balancers.
     * @param simTime The total simulation duration in clock cycles.
     */
    void printCombinedSummary(int simTime);

private:
    LoadBalancer *streamBalancer;  ///< Pointer to the streaming-specific load balancer
    LoadBalancer *processBalancer; ///< Pointer to the process-specific load balancer
    Logger &logger;                ///< Reference to the simulation logger
};

#endif