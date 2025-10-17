#include "HealthMonitor.hpp"
#include "ModuleManager.hpp"
#include <iostream>
#include <iomanip>

// Initialize static member
HealthMonitor* HealthMonitor::instance = nullptr;

HealthMonitor::HealthMonitor() 
    : monitoring(false),
      checkInterval(std::chrono::seconds(10)),
      failureThreshold(3),
      systemHealth(HealthStatus::HEALTHY) {
    
    auto& logger = Logger::getInstance();
    logger.info("Health Monitor initialized", "HealthMonitor");
}

HealthMonitor::~HealthMonitor() {
    stopMonitoring();
    
    auto& logger = Logger::getInstance();
    logger.info("Health Monitor destroyed", "HealthMonitor");
}

HealthMonitor& HealthMonitor::getInstance() {
    if (!instance) {
        instance = new HealthMonitor();
    }
    return *instance;
}

void HealthMonitor::startMonitoring() {
    if (monitoring) {
        return;
    }

    auto& logger = Logger::getInstance();
    logger.info("Starting health monitoring with interval: " + 
                std::to_string(checkInterval.count()) + "s", "HealthMonitor");

    monitoring = true;
    monitorThread = std::thread(&HealthMonitor::monitoringLoop, this);
}

void HealthMonitor::stopMonitoring() {
    if (!monitoring) {
        return;
    }

    auto& logger = Logger::getInstance();
    logger.info("Stopping health monitoring", "HealthMonitor");

    monitoring = false;
    if (monitorThread.joinable()) {
        monitorThread.join();
    }
}

bool HealthMonitor::isMonitoring() const {
    return monitoring;
}

void HealthMonitor::monitoringLoop() {
    auto& logger = Logger::getInstance();
    logger.debug("Health monitor loop started", "HealthMonitor");

    while (monitoring) {
        try {
            performHealthChecks();
            updateSystemHealth();
            checkForAlerts();
            logHealthStatus();
        } catch (const std::exception& e) {
            logger.error("Health monitor exception: " + std::string(e.what()), "HealthMonitor");
        }

        // run runtime shared-library scan and log results
        ModuleManager::getInstance().scanAndLogRuntimeSharedLibraries();

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    logger.debug("Health monitor loop stopped", "HealthMonitor");
}

void HealthMonitor::performHealthChecks() {
    std::lock_guard<std::mutex> lock(healthMutex);
    auto& logger = Logger::getInstance();

    for (auto& [moduleName, checkFunction] : healthChecks) {
        auto startTime = std::chrono::steady_clock::now();
        
        try {
            bool isHealthy = checkFunction();
            auto endTime = std::chrono::steady_clock::now();
            auto responseTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            HealthCheckResult result;
            result.lastCheck = endTime;
            result.responseTimeMs = responseTime.count();

            if (isHealthy) {
                result.status = HealthStatus::HEALTHY;
                result.message = "Module is healthy";
                result.consecutiveFailures = 0;
                
                logger.debug("Health check passed: " + moduleName + 
                            " (response: " + std::to_string(responseTime.count()) + "ms)", "HealthMonitor");
            } else {
                result.consecutiveFailures = healthStatus[moduleName].consecutiveFailures + 1;
                
                if (result.consecutiveFailures >= failureThreshold) {
                    result.status = HealthStatus::CRITICAL;
                    result.message = "Module critically unhealthy - " + 
                                   std::to_string(result.consecutiveFailures) + " consecutive failures";
                    
                    logger.error("Critical health failure: " + moduleName, "HealthMonitor");
                } else {
                    result.status = HealthStatus::UNHEALTHY;
                    result.message = "Module unhealthy - " + 
                                   std::to_string(result.consecutiveFailures) + " consecutive failures";
                    
                    logger.warning("Health check failed: " + moduleName, "HealthMonitor");
                }
            }
            
            healthStatus[moduleName] = result;

        } catch (const std::exception& e) {
            HealthCheckResult result;
            result.status = HealthStatus::CRITICAL;
            result.message = "Health check exception: " + std::string(e.what());
            result.lastCheck = std::chrono::steady_clock::now();
            result.consecutiveFailures = healthStatus[moduleName].consecutiveFailures + 1;
            result.responseTimeMs = -1;

            healthStatus[moduleName] = result;
            
            logger.error("Health check exception for " + moduleName + ": " + e.what(), "HealthMonitor");
        }
    }
}

void HealthMonitor::registerModule(const std::string& moduleName, 
                                  std::function<bool()> healthCheckFunction) {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto& logger = Logger::getInstance();
    logger.info("Registering health check for module: " + moduleName, "HealthMonitor");

    healthChecks[moduleName] = healthCheckFunction;
    
    // Initialize health status
    HealthCheckResult initialStatus;
    initialStatus.status = HealthStatus::HEALTHY;
    initialStatus.message = "Initialized - waiting for first health check";
    initialStatus.lastCheck = std::chrono::steady_clock::now();
    initialStatus.consecutiveFailures = 0;
    initialStatus.responseTimeMs = 0;
    
    healthStatus[moduleName] = initialStatus;

    // Initialize metrics
    ModuleMetrics initialMetrics{};
    initialMetrics.lastOperationTime = std::chrono::steady_clock::now();
    moduleMetrics[moduleName] = initialMetrics;
}

void HealthMonitor::unregisterModule(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto& logger = Logger::getInstance();
    logger.info("Unregistering health check for module: " + moduleName, "HealthMonitor");

    healthChecks.erase(moduleName);
    healthStatus.erase(moduleName);
    moduleMetrics.erase(moduleName);
}

HealthMonitor::HealthCheckResult HealthMonitor::getModuleHealth(const std::string& moduleName) const {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto it = healthStatus.find(moduleName);
    if (it != healthStatus.end()) {
        return it->second;
    }
    
    HealthCheckResult unknownStatus;
    unknownStatus.status = HealthStatus::UNHEALTHY;
    unknownStatus.message = "Module not registered for health monitoring";
    unknownStatus.lastCheck = std::chrono::steady_clock::now();
    unknownStatus.consecutiveFailures = 0;
    unknownStatus.responseTimeMs = 0;
    
    return unknownStatus;
}


void HealthMonitor::recordModuleLoad(const std::string& moduleName, 
                                    std::chrono::milliseconds loadTime) {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto& metrics = moduleMetrics[moduleName];
    metrics.totalLoads++;
    metrics.lastOperationTime = std::chrono::steady_clock::now();
    
    // Update average load time
    if (metrics.totalLoads == 1) {
        metrics.averageLoadTime = loadTime;
    } else {
        metrics.averageLoadTime = std::chrono::milliseconds(
            (metrics.averageLoadTime.count() * (metrics.totalLoads - 1) + loadTime.count()) / metrics.totalLoads
        );
    }

    auto& logger = Logger::getInstance();
    logger.debug("Recorded module load: " + moduleName + 
                " (time: " + std::to_string(loadTime.count()) + "ms)", "HealthMonitor");
}

void HealthMonitor::recordModuleUnload(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto& metrics = moduleMetrics[moduleName];
    metrics.totalUnloads++;
    metrics.lastOperationTime = std::chrono::steady_clock::now();

    auto& logger = Logger::getInstance();
    logger.debug("Recorded module unload: " + moduleName, "HealthMonitor");
}

void HealthMonitor::recordHotSwap(const std::string& moduleName, bool success) {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto& metrics = moduleMetrics[moduleName];
    metrics.totalHotSwaps++;
    metrics.lastOperationTime = std::chrono::steady_clock::now();
    
    if (!success) {
        metrics.failedOperations++;
    }

    auto& logger = Logger::getInstance();
    logger.debug("Recorded hot-swap: " + moduleName + " (success: " + 
                (success ? "yes" : "no") + ")", "HealthMonitor");
}

HealthMonitor::ModuleMetrics HealthMonitor::getModuleMetrics(const std::string& moduleName) const {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    auto it = moduleMetrics.find(moduleName);
    if (it != moduleMetrics.end()) {
        return it->second;
    }
    
    return ModuleMetrics{};
}

void HealthMonitor::updateSystemHealth() {
    std::lock_guard<std::mutex> lock(healthMutex);
    
    int criticalCount = 0;
    int unhealthyCount = 0;
    int healthyCount = 0;

    for (const auto& [moduleName, health] : healthStatus) {
        switch (health.status) {
            case HealthStatus::CRITICAL:
                criticalCount++;
                break;
            case HealthStatus::UNHEALTHY:
                unhealthyCount++;
                break;
            case HealthStatus::DEGRADED:
                unhealthyCount++;
                break;
            case HealthStatus::HEALTHY:
                healthyCount++;
                break;
        }
    }

    HealthStatus newSystemHealth;
    if (criticalCount > 0) {
        newSystemHealth = HealthStatus::CRITICAL;
    } else if (unhealthyCount > 0) {
        newSystemHealth = HealthStatus::DEGRADED;
    } else if (healthyCount > 0) {
        newSystemHealth = HealthStatus::HEALTHY;
    } else {
        newSystemHealth = HealthStatus::UNHEALTHY; // No modules registered
    }

    // Log system health changes
    if (newSystemHealth != systemHealth) {
        auto& logger = Logger::getInstance();
        
        std::string healthMessage = "System health changed: ";
        switch (newSystemHealth) {
            case HealthStatus::HEALTHY: healthMessage += "HEALTHY"; break;
            case HealthStatus::DEGRADED: healthMessage += "DEGRADED"; break;
            case HealthStatus::UNHEALTHY: healthMessage += "UNHEALTHY"; break;
            case HealthStatus::CRITICAL: healthMessage += "CRITICAL"; break;
        }
        healthMessage += " (healthy: " + std::to_string(healthyCount) +
                        ", degraded: " + std::to_string(unhealthyCount) +
                        ", critical: " + std::to_string(criticalCount) + ")";
        
        logger.info(healthMessage, "HealthMonitor");
    }

    systemHealth = newSystemHealth;
    lastSystemCheck = std::chrono::steady_clock::now();
}

HealthMonitor::HealthStatus HealthMonitor::getSystemHealth() const {
    return systemHealth;
}

void HealthMonitor::checkForAlerts() {
    std::lock_guard<std::mutex> lock(healthMutex);
    auto& logger = Logger::getInstance();

    for (const auto& [moduleName, health] : healthStatus) {
        if (health.status == HealthStatus::CRITICAL) {
            logger.critical("CRITICAL ALERT: Module " + moduleName + " - " + health.message, "HealthMonitor");
        }
    }

    if (systemHealth == HealthStatus::CRITICAL) {
        logger.critical("SYSTEM CRITICAL ALERT: System health is CRITICAL", "HealthMonitor");
    }
}

void HealthMonitor::logHealthStatus() {
    auto& logger = Logger::getInstance();
    
    std::lock_guard<std::mutex> lock(healthMutex);
    
    std::string statusMessage = "Health Status - ";
    switch (systemHealth) {
        case HealthStatus::HEALTHY: statusMessage += "HEALTHY"; break;
        case HealthStatus::DEGRADED: statusMessage += "DEGRADED"; break;
        case HealthStatus::UNHEALTHY: statusMessage += "UNHEALTHY"; break;
        case HealthStatus::CRITICAL: statusMessage += "CRITICAL"; break;
    }
    
    statusMessage += " | Modules: " + std::to_string(healthStatus.size()) +
                    " | Monitoring: " + (monitoring ? "ACTIVE" : "INACTIVE");
    
    logger.info(statusMessage, "HealthMonitor");
}

void HealthMonitor::generateHealthReport() const {
    auto& logger = Logger::getInstance();
    
    std::lock_guard<std::mutex> lock(healthMutex);
    
    logger.info("=== SYSTEM HEALTH REPORT ===", "HealthMonitor");
    
    // System health
    std::string systemHealthStr;
    switch (systemHealth) {
        case HealthStatus::HEALTHY: systemHealthStr = "HEALTHY"; break;
        case HealthStatus::DEGRADED: systemHealthStr = "DEGRADED"; break;
        case HealthStatus::UNHEALTHY: systemHealthStr = "UNHEALTHY"; break;
        case HealthStatus::CRITICAL: systemHealthStr = "CRITICAL"; break;
    }
    logger.info("System Health: " + systemHealthStr, "HealthMonitor");
    
    // Module health details
    for (const auto& [moduleName, health] : healthStatus) {
        std::string moduleStatus;
        switch (health.status) {
            case HealthStatus::HEALTHY: moduleStatus = "HEALTHY"; break;
            case HealthStatus::DEGRADED: moduleStatus = "DEGRADED"; break;
            case HealthStatus::UNHEALTHY: moduleStatus = "UNHEALTHY"; break;
            case HealthStatus::CRITICAL: moduleStatus = "CRITICAL"; break;
        }
        
        auto timeSinceCheck = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - health.lastCheck);
            
        logger.info("Module: " + moduleName + " | Status: " + moduleStatus +
                   " | Response: " + std::to_string(health.responseTimeMs) + "ms" +
                   " | Failures: " + std::to_string(health.consecutiveFailures) +
                   " | Last check: " + std::to_string(timeSinceCheck.count()) + "s ago", 
                   "HealthMonitor");
    }
    
    logger.info("=== END HEALTH REPORT ===", "HealthMonitor");
}

void HealthMonitor::setCheckInterval(std::chrono::seconds interval) {
    checkInterval = interval;
    
    auto& logger = Logger::getInstance();
    logger.info("Health check interval set to: " + std::to_string(interval.count()) + "s", "HealthMonitor");
}

void HealthMonitor::setFailureThreshold(int threshold) {
    failureThreshold = threshold;
    
    auto& logger = Logger::getInstance();
    logger.info("Failure threshold set to: " + std::to_string(threshold), "HealthMonitor");
}

