/**
 * @file LoadBalancer.cpp
 * @brief Implementation of the LoadBalancer class, managing a dynamic pool of web servers and a request queue.
 */

#include "LoadBalancer.h"
#include "Utils.h"
#include <sstream>

/**
 * @brief Constructs a LoadBalancer instance, parsing configuration parameters and setting up the firewall.
 * @param name_ The identifying name for this load balancer.
 * @param cfg_ Reference to the Configuration object containing simulation parameters.
 * @param logger_ Reference to the shared Logger utility.
 */
LoadBalancer::LoadBalancer(const std::string &name_, const Configuration &cfg_, Logger &logger_): 
    name(name_), requestQueue(nullptr), cfg(cfg_), logger(logger_),
    minQueueFactor(cfg_.getInt("minQueueFactor",50)),
    maxQueueFactor(cfg_.getInt("maxQueueFactor",80)),
    restCycles(cfg_.getInt("restCycles",10)),
    lastScaleTick(0),
    initialQueuePerServer(cfg_.getInt("initialQueuePerServer",100)),
    arrivalRate(cfg_.getInt("arrivalRate",2)),
    nextRequestId(0),
    totalGenerated(0),
    totalProcessed(0), 
    totalBlocked(0), 
    totalAdded(0), 
    totalRemoved(0), 
    procCount(0),
    streamCount(0),
    active(0)
{
    std::string blocked = cfg_.getString("blockedPrefix", "");
    if (!blocked.empty()) {
        std::istringstream ss(blocked);
        std::string part;
        while (std::getline(ss, part, ',')) {
            firewall.addBlockedPrefix(part);
        }
    }
}

/**
 * @brief Default destructor for the LoadBalancer.
 */
LoadBalancer::~LoadBalancer() {}

/**
 * @brief Initializes the load balancer with a starting number of servers and assigns its request queue.
 * @param numServers The starting number of web servers to instantiate.
 * @param sharedQueue Reference to the RequestQueue to draw tasks from.
 */
void LoadBalancer::initialize(int numServers, RequestQueue &sharedQueue) {
    requestQueue = &sharedQueue;
    peakServers = numServers;
    minServers = numServers;
    initialServers = numServers;
    for (int i = 0;i < numServers; i++) {
        addServer();
    }
    totalAdded = 0;
}

/**
 * @brief Instantiates a new WebServer and adds it to the active pool, updating scaling metrics.
 */
void LoadBalancer::addServer() {
    int id = (int)servers.size() + 1;
    servers.emplace_back(std::make_unique<WebServer>(id));
    totalAdded++;
    peakServers = std::max(peakServers, (int)servers.size());
}

/**
 * @brief Removes an idle web server from the pool to scale down resources.
 */
void LoadBalancer::removeServer() {
    if (servers.size() > 1){
        for (auto it = servers.rbegin(); it != servers.rend(); ++it) {
            if ((*it)->isIdle()) {
                servers.erase(std::next(it).base());
                totalRemoved++;
                logger.log("[" + name + " SCALE DOWN] Queue size (" + std::to_string(requestQueue->size()) + ") dropped below threshold (" + std::to_string(minQueueFactor * (servers.size() + 1)) + "). New server count: " + std::to_string(servers.size()));
                minServers = std::min(minServers, (int)servers.size());
                return; 
            }
        }
    }
}

/**
 * @brief Iterates through the server pool and assigns queued requests to any available idle servers.
 */
void LoadBalancer::distributeRequests() {
    if (!requestQueue) {
        return;
    }
    for (auto &s : servers) {
        if (s->isIdle() && !requestQueue->isEmpty()) {
            Request r = requestQueue->dequeue();
            s->assignRequest(r);
            active++;
        }
    }
}

/**
 * @brief Advances the clock cycle for all active web servers, tracking completion of tasks.
 * @param currentTick The current simulation time unit.
 */
void LoadBalancer::updateServers(int currentTick) {
    for (auto &s : servers) {
        bool finished = s->processOneCycle();
        if (finished) {
            totalProcessed++;
            active--;
        }
    }
}

/**
 * @brief Evaluates current queue size metrics and triggers scaling events (add/remove server) if necessary.
 */
void LoadBalancer::checkScaling() {
    if (!requestQueue) {
        return;
    }
    int qsize = (int)requestQueue->size();
    int serversCount = (int)servers.size();
    if (qsize > maxQueueFactor * serversCount) {
        addServer();
        int threshold = maxQueueFactor * (serversCount - 1);
        logger.log("[" + name + " SCALE UP] Queue size (" + std::to_string(qsize) + ") exceeded threshold (" + std::to_string(threshold) + "). New server count: " + std::to_string(servers.size()));
    } 
    else if (qsize < minQueueFactor * serversCount) {
        removeServer();
    }
}

/**
 * @brief Receives an incoming request, checks it against the firewall, and enqueues it if allowed.
 * @param r The Request object attempting to enter the load balancer.
 * @return true if the request was successfully queued, false if it was blocked by the firewall.
 */
bool LoadBalancer::acceptRequest(const Request &r) {
    if (firewall.isBlocked(r.ipIn)) {
        totalBlocked++;
        return false;
    }
    if (requestQueue) {
        requestQueue->enqueue(r);
    }
    return true;
}

/**
 * @brief Retrieves the assigned name of this LoadBalancer instance.
 * @return A string representing the load balancer's name.
 */
std::string LoadBalancer::getName() const { 
    return name; 
}

/**
 * @brief Increments the counter tracking the number of streaming ('S') requests processed.
 */
void LoadBalancer::incStreamCount() {
    streamCount++;
}

/**
 * @brief Increments the counter tracking the number of processing ('P') requests processed.
 */
void LoadBalancer::incProcCount(){
    procCount++;
}

/**
 * @brief Executes the main simulation loop for standalone LoadBalancer execution.
 * @param maxTime The maximum number of clock cycles to run the simulation.
 */
void LoadBalancer::runSimulation(int maxTime) {
    if (!requestQueue) {
        return;
    }
    int initialFill = cfg.getInt("initialQueuePerServer", initialQueuePerServer) * (int)servers.size();
    long localId = nextRequestId;
    for (int i = 0; i < initialFill; i++) {
        Request r(randomIp(), randomIp(), randBetween(1, cfg.getInt("maxRequestTime",10)), (randBetween(0,1)==0 ? 'P' : 'S'), localId++);
        totalGenerated++;
        requestQueue->enqueue(r);
    }
    nextRequestId = localId;

    for (int tick = 0; tick < maxTime; ++tick) {
        int newReqs = randBetween(0, arrivalRate);
        for (int n = 0; n < newReqs; n++) {
            Request r(randomIp(), randomIp(), randBetween(1, cfg.getInt("maxRequestTime",10)), (randBetween(0,1)==0 ? 'P' : 'S'), nextRequestId++);
            totalGenerated++;
            acceptRequest(r);
        }

        distributeRequests();
        updateServers(tick);

        if (tick % 1000 == 0 && tick != 0) {
            int total = servers.size();
            double utilization = (double)active / total * 100;
            logger.log("");
            logger.log("---- SNAPSHOT @ " + std::to_string(tick) + " ----");
            logger.log("Queue Size: " + std::to_string(requestQueue->size()));
            logger.log("Utilization: " + std::to_string(active) + "/" + std::to_string(total) + " (" + std::to_string(utilization) + "%)");
            logger.log("Total Processed: " + std::to_string(totalProcessed));
            logger.log("Blocked by Firewall: " + std::to_string(totalBlocked));
            logger.log("");
        }

        if (tick % cfg.getInt("restCycles",10) == 0 && tick != 0) {
            checkScaling();
            lastScaleTick = tick;
        }
    }
}

/**
 * @brief Logs the comprehensive final metrics for this load balancer at the end of the simulation.
 * @param simTime The total time in clock cycles that the simulation ran.
 */
void LoadBalancer::printSummary(int simTime) {
    logger.log("");
    logger.log("=========================================");
    logger.log("      LOAD BALANCER FINAL SUMMARY        ");
    logger.log("=========================================");
    logger.log("Simulation Time: " + std::to_string(simTime) + " clock cycles");
    logger.log("Initial Servers: " + std::to_string(initialServers));
    logger.log("Final Servers: " + std::to_string(servers.size()));
    logger.log("Peak Servers: " + std::to_string(peakServers));
    logger.log("Min Servers: " + std::to_string(minServers));
    logger.log("");
    logger.log("Requests Metrics:");
    logger.log("-----------------");
    logger.log("Total Generated: " + std::to_string(totalGenerated));
    logger.log("Total Processed: " + std::to_string(totalProcessed));
    logger.log("Left in Queue: " + std::to_string(requestQueue->size()));
    logger.log("Blocked by Firewall: " + std::to_string(totalBlocked));
    int inProcess = totalGenerated - totalProcessed - requestQueue->size() - totalBlocked;
    logger.log("In Process: " + std::to_string(inProcess));
    logger.log("");
    logger.log("Scaling Events:");
    logger.log("---------------");
    logger.log("Servers Added: " + std::to_string(totalAdded) + " times");
    logger.log("Servers Removed: " + std::to_string(totalRemoved) + " times");
    logger.log("=========================================");
}

/**
 * @brief Logs a snapshot of the load balancer's current metrics at a specific timestamp.
 * @param t The current simulation clock cycle timestamp.
 */
void LoadBalancer::printSnapshot(int t) {
    double utilization = 0.0;
    if (servers.size() > 0) {
        utilization = (double)active / servers.size() * 100.0;
    }

    logger.log("");
    logger.log("---- " + name + " SNAPSHOT @ " + std::to_string(t) +  " ----");
    logger.log("Queue Size: " + std::to_string(requestQueue->size()));
    logger.log("Utilization: " + std::to_string(active) + "/" + std::to_string(servers.size()) + " (" + std::to_string(utilization) + "%)");
    logger.log("Total Processed: " + std::to_string(totalProcessed));
    logger.log("Blocked by Firewall: " + std::to_string(totalBlocked));
    logger.log("");
}