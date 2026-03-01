#include <iostream>
#include "Configuration.h"
#include "Logger.h"
#include "RequestQueue.h"
#include "LoadBalancer.h"
#include "JobSwitch.h"
#include "Utils.h"

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
