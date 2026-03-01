/**
 * @file JobSwitch.cpp
 * @brief Implementation of the JobSwitch class that routes requests to specialized load balancers.
 */

#include "JobSwitch.h"
#include "LoadBalancer.h"
#include <sstream>

/**
 * @brief Constructs a JobSwitch instance to route incoming requests.
 * * @param streamLB Pointer to the LoadBalancer handling streaming ('S') requests.
 * @param procLB Pointer to the LoadBalancer handling processing ('P') requests.
 * @param logger_ Reference to the Logger instance used to record simulation metrics.
 */
JobSwitch::JobSwitch(LoadBalancer *streamLB, LoadBalancer *procLB, Logger &logger_): streamBalancer(streamLB), processBalancer(procLB), logger(logger_) {}

/**
 * @brief Evaluates the job type of an incoming request and routes it to the correct load balancer.
 * * If the request's job type is 'S', it routes to the streaming balancer. 
 * Otherwise, it routes to the processing balancer. It also increments the 
 * respective job counters on the chosen load balancer.
 * * @param r The incoming Request object to be evaluated and routed.
 */
void JobSwitch::routeRequest(const Request &r) {
    if (r.jobType == 'S') {
        if (streamBalancer) {
            streamBalancer->acceptRequest(r);
            streamBalancer->incStreamCount();
        }
    } 
    else {
        if (processBalancer) {
            processBalancer->acceptRequest(r);
            processBalancer->incProcCount();
        }
    }
}

/**
 * @brief Aggregates and prints a comprehensive summary of all managed load balancers.
 * * Calculates combined metrics including server counts, total requests processed, 
 * requests currently in queue, blocked requests, and overall scaling events.
 * * @param simTime The total simulation duration in clock cycles.
 */
void JobSwitch::printCombinedSummary(int simTime) {
    logger.log("");
    logger.log("=========================================");
    logger.log("            JOB SWITCH SUMMARY           ");
    logger.log("=========================================");
    logger.log("Simulation Time: " + std::to_string(simTime) + " clock cycles");
    logger.log("Total Initial Servers: " + std::to_string(streamBalancer->getInitialServers() + processBalancer->getInitialServers()));
    logger.log("Total Final Servers: " + std::to_string(streamBalancer->getFinalServers() + processBalancer->getFinalServers()));
    logger.log("Combined Peak Servers: " + std::to_string(streamBalancer->getPeakServers() + processBalancer->getPeakServers()));
    logger.log("");
    logger.log("Requests Metrics:");
    logger.log("-----------------");
    logger.log("Stream Requests: " + std::to_string(streamBalancer->getStreamCount()));
    logger.log("Process Requests: " + std::to_string(processBalancer->getProcCount()));
    logger.log("Total Processed: " + std::to_string(streamBalancer->getTotalProcessed() + processBalancer->getTotalProcessed()));
    logger.log("Total Left in Queue: " + std::to_string(streamBalancer->getQueueSize() + processBalancer->getQueueSize()));
    logger.log("Blocked by Firewall: " + std::to_string(streamBalancer->getTotalBlocked() + processBalancer->getTotalBlocked()));
    int inProcess = streamBalancer->getStreamCount() + processBalancer->getProcCount() - (streamBalancer->getTotalProcessed() + processBalancer->getTotalProcessed()) - (streamBalancer->getQueueSize() + processBalancer->getQueueSize()) - (streamBalancer->getTotalBlocked() + processBalancer->getTotalBlocked());
    logger.log("In Process: " + std::to_string(inProcess));
    logger.log("");
    logger.log("Scaling Events:");
    logger.log("---------------");
    logger.log("Total Servers Added: " + std::to_string(streamBalancer->getTotalAdded() + processBalancer->getTotalAdded()) + " times");
    logger.log("Total Servers Removed: " + std::to_string(streamBalancer->getTotalRemoved() + processBalancer->getTotalRemoved()) + " times");
    logger.log("=========================================");
}