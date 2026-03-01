#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "RequestQueue.h"
#include "WebServer.h"
#include "Firewall.h"
#include "Configuration.h"
#include "Logger.h"
#include <vector>
#include <memory>

/**
 * @file LoadBalancer.h
 * @brief LoadBalancer manages servers, queue, scaling, and simulation loop.
 */

/**
 * @class LoadBalancer
 * @brief Manages a pool of web servers, handles request queuing, and scales servers based on load.
 */
class LoadBalancer {
public:
    /**
     * @brief Constructs a LoadBalancer instance.
     * @param name The descriptive name of the load balancer.
     * @param cfg Reference to the simulation configuration properties.
     * @param logger Reference to the shared logging utility.
     */
    LoadBalancer(const std::string &name, const Configuration &cfg, Logger &logger);

    /**
     * @brief Destructor for LoadBalancer.
     */
    ~LoadBalancer();

    /**
     * @brief Initializes the load balancer with a starting number of servers and assigns a queue.
     * @param initialServers The starting number of active web servers.
     * @param sharedQueue Reference to the request queue this load balancer will draw from.
     */
    void initialize(int initialServers, RequestQueue &sharedQueue);

    /**
     * @brief Runs the main simulation loop for this load balancer for a given time.
     * @param maxTime The total number of clock cycles to simulate.
     */
    void runSimulation(int maxTime);

    /**
     * @brief Scales up the server pool by adding one new active web server.
     */
    void addServer();

    /**
     * @brief Scales down the server pool by removing one idle web server.
     */
    void removeServer();

    /**
     * @brief Assigns pending requests from the queue to any available idle servers.
     */
    void distributeRequests();

    /**
     * @brief Advances the clock by one cycle for all managed web servers.
     * @param currentTick The current simulation clock cycle.
     */
    void updateServers(int currentTick);

    /**
     * @brief Evaluates current queue size against server capacity to trigger scaling events.
     */
    void checkScaling();

    /**
     * @brief Outputs a detailed summary of the load balancer's performance metrics.
     * @param simTime The total simulation time completed.
     */
    void printSummary(int simTime);

    /**
     * @brief Outputs a snapshot of the load balancer's performance metrics.
     * @param t The time the snapshot was taken.
     */
    void printSnapshot(int t);

    /**
     * @brief Evaluates and accepts a direct incoming request (used by JobSwitch).
     * @param r The incoming request to evaluate.
     * @return true if the request is accepted, false if blocked by the firewall.
     */
    bool acceptRequest(const Request &r);

    /**
     * @brief Gets the name of the load balancer.
     * @return A string representing the name.
     */
    std::string getName() const;

    /**
     * @brief Increments the counter for streaming jobs handled.
     */
    void incStreamCount();

    /**
     * @brief Increments the counter for processing jobs handled.
     */
    void incProcCount();

    /**
     * @brief Retrieves the starting count of initialized servers.
     * @return Initial server count.
     */
    int getInitialServers() const { return initialServers; }

    /**
     * @brief Retrieves the current count of active servers.
     * @return Final/Current server count.
     */
    int getFinalServers() const { return servers.size(); }

    /**
     * @brief Retrieves the peak maximum number of servers active simultaneously.
     * @return Peak server count.
     */
    int getPeakServers() const { return peakServers; }

    /**
     * @brief Retrieves the minimum recorded number of active servers.
     * @return Minimum server count.
     */
    int getMinServers() const { return minServers; }

    /**
     * @brief Retrieves the total count of streaming jobs.
     * @return Stream job count.
     */
    int getStreamCount() const { return streamCount; }

    /**
     * @brief Retrieves the total count of processing jobs.
     * @return Processing job count.
     */
    int getProcCount() const { return procCount; }

    /**
     * @brief Retrieves the total number of successfully processed requests.
     * @return Total processed count.
     */
    int getTotalProcessed() const { return totalProcessed; }

    /**
     * @brief Retrieves the current amount of requests left in the queue.
     * @return Current queue size.
     */
    int getQueueSize() const { return requestQueue ? requestQueue->size() : 0; }

    /**
     * @brief Retrieves the total number of requests blocked by the firewall.
     * @return Total blocked count.
     */
    int getTotalBlocked() const { return totalBlocked; }

    /**
     * @brief Retrieves the total number of servers added during scaling.
     * @return Total added count.
     */
    int getTotalAdded() const { return totalAdded; }

    /**
     * @brief Retrieves the total number of servers removed during scaling.
     * @return Total removed count.
     */
    int getTotalRemoved() const { return totalRemoved; }

private:
    std::string name;                                ///< The name identifier of the load balancer
    RequestQueue *requestQueue;                      ///< Pointer to shared queue (or own)
    std::vector<std::unique_ptr<WebServer>> servers; ///< Collection of active web servers
    Firewall firewall;                               ///< Firewall checking incoming request IPs
    const Configuration &cfg;                        ///< Reference to simulation configuration
    Logger &logger;                                  ///< Reference to simulation logger

    int minQueueFactor;                              ///< Multiplier threshold for scaling down
    int maxQueueFactor;                              ///< Multiplier threshold for scaling up
    int restCycles;                                  ///< Cooldown cycles required between scaling events
    int lastScaleTick;                               ///< Simulation tick of the last scaling event
    int initialQueuePerServer;                       ///< Number of requests to generate per server on start
    int arrivalRate;                                 ///< Maximum expected new requests generated per cycle
    long nextRequestId;                              ///< Auto-incrementing ID for generated requests

    int totalGenerated = 0;                          ///< Total requests created
    int totalProcessed = 0;                          ///< Total requests finished
    int totalBlocked = 0;                            ///< Total requests denied by firewall
    int totalAdded = 0;                              ///< Total servers spawned
    int totalRemoved = 0;                            ///< Total servers destroyed
    int peakServers = 0;                             ///< Highest number of concurrent servers recorded
    int minServers = 0;                              ///< Lowest allowed number of servers
    int initialServers = 0;                          ///< Starting number of servers
    int procCount = 0;                               ///< Count of Process-type jobs routed
    int streamCount = 0;                             ///< Count of Stream-type jobs routed
    int active = 0;                                  ///< Count of activer servers
};
#endif