#include <iostream>
#include "Configuration.h"
#include "Logger.h"
#include "RequestQueue.h"
#include "LoadBalancer.h"
#include "JobSwitch.h"
#include "Utils.h"

/**
 * @mainpage Load Balancer Simulation Documentation
 *
 * @section intro_sec Introduction
 * Welcome to the documentation for the Load Balancer Simulation project. 
 * This system is designed to simulate how network traffic is distributed 
 * across a dynamic pool of web servers.
 *
 * @section desc_sec Project Description
 * The simulation handles thousands of clock cycles, generating incoming 
 * requests and routing them based on job type ('S' for Streaming, 'P' for Processing). 
 * It features auto-scaling logic to add or remove servers based on queue capacity, 
 * as well as a basic IP firewall.
 *
 * @section notes_sec Notes & Comments
 * - **Scaling:** Managed by `minQueueFactor` and `maxQueueFactor`.
 * - **Metrics:** A final snapshot is generated in user specified log file upon completion.
 * - **Author:** Tanish Bandari
 */


/**
 * @brief The main execution function driving the load balancer simulation.
 * * This function initializes the simulation environment by reading properties from a 
 * configuration file and setting up the logging utility. Based on the configuration, 
 * it determines whether to run the simulation using a dual-balancer setup routed by 
 * a JobSwitch, or a single standard LoadBalancer. It then executes the simulation 
 * loop for the configured number of clock cycles, handling request generation, 
 * routing, server updates, scaling checks, and snapshot logging.
 * * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments. `argv[1]` can optionally be used to specify a custom configuration file path.
 * @return 0 upon successful completion of the simulation, 1 if the configuration file fails to load.
 */
int main(int argc, char** argv) {
    std::string cfgPath = "config.cfg";
    if (argc > 1) {
        cfgPath = argv[1];
    }

    Configuration cfg;
    if (!cfg.loadFromFile(cfgPath)) {
        std::cout << "Failed to open config file: " << cfgPath << std::endl;
        return 1;
    }

    Logger logger;
    std::string logname = cfg.getString("logFile", "simulation.log");
    logger.open(logname);

    int numServers = cfg.getInt("numServers", 10);
    int simTime = cfg.getInt("simulationTime", 10000);
    bool useJobSwitch = cfg.getInt("useJobSwitch", 0);

    RequestQueue streamQ;
    RequestQueue procQ;
    RequestQueue q;

    logger.log("===== SIMULATION START =====");
    logger.log("Initial Servers: " + std::to_string(numServers));
    logger.log("Starting Queue Size: " + std::to_string(numServers * cfg.getInt("initialQueuePerServer", 100)));
    logger.log("Task Time Range: 1 to " + std::to_string(cfg.getInt("maxRequestTime",10)));
    logger.log("============================");
    logger.log("");

    if (useJobSwitch) {
        LoadBalancer streamLB("StreamLB", cfg, logger);
        LoadBalancer procLB("ProcessLB", cfg, logger);
        streamLB.initialize(numServers/2, streamQ);
        procLB.initialize(numServers - numServers/2, procQ);
        JobSwitch sw(&streamLB, &procLB, logger);

        int initialFill = cfg.getInt("initialQueuePerServer", 100) * numServers;
        long localId = 0;
        for (int i = 0; i < initialFill; i++) {
            Request r(randomIp(), randomIp(), randBetween(1, cfg.getInt("maxRequestTime",10)), (randBetween(0,1)==0 ? 'P' : 'S'), localId++);
            sw.routeRequest(r);
        }

        for (int t = 0; t < simTime; t++) {
            int newreq = randBetween(0, cfg.getInt("arrivalRate", 2));
            for (int i = 0; i < newreq; ++i) {
                Request r(randomIp(), randomIp(), randBetween(1, cfg.getInt("maxRequestTime",10)), (randBetween(0,1)==0 ? 'P' : 'S'), localId++);
                sw.routeRequest(r);
            }
            streamLB.distributeRequests();
            procLB.distributeRequests();
            streamLB.updateServers(t);
            procLB.updateServers(t);

            if (t % 1000 == 0 && t != 0) { 
                streamLB.printSnapshot(t);
                procLB.printSnapshot(t);
            }

            if (t % cfg.getInt("restCycles",10) == 0 && t != 0) {
                streamLB.checkScaling();
                procLB.checkScaling();
            }
        }
        sw.printCombinedSummary(simTime);
    } 
    else {
        LoadBalancer lb("MainLB", cfg, logger);
        lb.initialize(numServers, q);
        lb.runSimulation(simTime);
        lb.printSummary(simTime);
    }

    logger.close();
    std::cout << "\033[32m" << "Simulation complete. Log: " << logname << std::endl;
    return 0;
}
