#pragma once
#include <atomic>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <string>
#include <mutex>
#include "../utils/Logger.hpp"

class ModuleManager; // Forward declaration

class HealthMonitor {
public:
    // Health status levels
    enum class HealthStatus {
        HEALTHY,
        DEGRADED, 
        UNHEALTHY,
        CRITICAL
    };

    // Health check result
    struct HealthCheckResult {
        HealthStatus status;
        std::string message;
        std::chrono::steady_clock::time_point lastCheck;
        int consecutiveFailures;
        double responseTimeMs;
    };

    // Metrics structure
    struct ModuleMetrics {
        size_t totalLoads;
        size_t totalUnloads;
        size_t totalHotSwaps;
        size_t failedOperations;
        std::chrono::milliseconds totalUptime;
        std::chrono::milliseconds averageLoadTime;
        std::chrono::steady_clock::time_point lastOperationTime;
    };

    // Singleton instance
    static HealthMonitor& getInstance();

    // Lifecycle management
    void startMonitoring();
    void stopMonitoring();
    bool isMonitoring() const;

    // Module health management
    void registerModule(const std::string& moduleName, 
                       std::function<bool()> healthCheckFunction);
    void unregisterModule(const std::string& moduleName);
    HealthCheckResult getModuleHealth(const std::string& moduleName) const;
    
    // Metrics management
    void recordModuleLoad(const std::string& moduleName, 
                         std::chrono::milliseconds loadTime);
    void recordModuleUnload(const std::string& moduleName);
    void recordHotSwap(const std::string& moduleName, bool success);
    ModuleMetrics getModuleMetrics(const std::string& moduleName) const;

    // System-wide health
    HealthStatus getSystemHealth() const;
    void generateHealthReport() const;

    // Configuration
    void setCheckInterval(std::chrono::seconds interval);
    void setFailureThreshold(int threshold);

    // Prevent copying
    HealthMonitor(const HealthMonitor&) = delete;
    HealthMonitor& operator=(const HealthMonitor&) = delete;

private:
    HealthMonitor();
    ~HealthMonitor();

    void monitoringLoop();
    void performHealthChecks();
    void updateSystemHealth();
    void checkForAlerts();
    void logHealthStatus();

    // Member variables
    static HealthMonitor* instance;
    
    std::atomic<bool> monitoring;
    std::thread monitorThread;
    std::chrono::seconds checkInterval;
    int failureThreshold;

    mutable std::mutex healthMutex;
    std::unordered_map<std::string, std::function<bool()>> healthChecks;
    std::unordered_map<std::string, HealthCheckResult> healthStatus;
    std::unordered_map<std::string, ModuleMetrics> moduleMetrics;
    
    HealthStatus systemHealth;
    std::chrono::steady_clock::time_point lastSystemCheck;
};