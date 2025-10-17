#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include "../src/core/ModuleManager.hpp"
#include "../src/core/HealthMonitor.hpp"

class UnstableModule : public IModule {
private:
    std::string name;
    std::string version;
    bool running;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    UnstableModule(const std::string& moduleName = "UnstableModule", 
                   const std::string& moduleVersion = "1.0")
        : name(moduleName), version(moduleVersion), running(false), gen(rd()), dis(1, 10) {
        std::cout << "UnstableModule constructed: " << name << std::endl;
    }

    virtual ~UnstableModule() {
        std::cout << "UnstableModule destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "UnstableModule initializing: " << name << std::endl;
        return true;
    }

    bool start() override {
        std::cout << "UnstableModule starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "UnstableModule stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "UnstableModule cleaning: " << name << std::endl;
        return true;
    }

    std::string getName() override {
        return name;
    }

    std::string getVersion() override {
        return version;
    }

    bool isHealthy() override {
        // Randomly fail to simulate unstable behavior
        int randomValue = dis(gen);
        bool healthy = randomValue > 3; // 70% chance of being healthy
        
        if (!healthy) {
            std::cout << "UnstableModule: Health check FAILED (random: " << randomValue << ")" << std::endl;
        }
        
        return healthy;
    }
};

// Factory functions for UnstableModule
extern "C" {
    IModule* createModule() {
        return new UnstableModule("UnstableModule", "1.0");
    }
    
    void destroyModule(IModule* module) {
        if (module) {
            delete module;
        }
    }
    
    const char* getModuleType() {
        return "Unstable";
    }
    
    int getModuleVersion() {
        return 1;
    }
}

void demonstrateHealthMonitoring() {
    auto& healthMonitor = HealthMonitor::getInstance();
    auto& manager = ModuleManager::getInstance();
    
    std::cout << "=== HEALTH MONITORING DEMONSTRATION ===" << std::endl;
    
    // Configure health monitor
    healthMonitor.setCheckInterval(std::chrono::seconds(5));
    healthMonitor.setFailureThreshold(2);
    
    // Start health monitoring
    healthMonitor.startMonitoring();
    
    std::cout << "\n1. Loading stable module (Calculator V1)..." << std::endl;
    manager.loadModule("./calculator_v1.so");
    
    std::cout << "\n2. Loading unstable module..." << std::endl;
    manager.loadModule("./unstable_module.so");
    
    std::cout << "\n3. Monitoring system for 30 seconds..." << std::endl;
    std::cout << "   Health monitor will check modules every 5 seconds" << std::endl;
    std::cout << "   Unstable module will randomly fail health checks" << std::endl;
    
    for (int i = 0; i < 30; i++) {
        std::cout << "Monitoring... " << (i + 1) << "s" << std::endl;
        
        // Generate health report every 10 seconds
        if ((i + 1) % 10 == 0) {
            healthMonitor.generateHealthReport();
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\n4. Final health report..." << std::endl;
    healthMonitor.generateHealthReport();
    
    std::cout << "\n5. Checking individual module health..." << std::endl;
    auto calculatorHealth = healthMonitor.getModuleHealth("Calculator");
    auto unstableHealth = healthMonitor.getModuleHealth("UnstableModule");
    
    std::cout << "Calculator health: " << calculatorHealth.message << std::endl;
    std::cout << "UnstableModule health: " << unstableHealth.message << std::endl;
    std::cout << "System health: ";
    
    switch (healthMonitor.getSystemHealth()) {
        case HealthMonitor::HealthStatus::HEALTHY:
            std::cout << "HEALTHY" << std::endl;
            break;
        case HealthMonitor::HealthStatus::DEGRADED:
            std::cout << "DEGRADED" << std::endl;
            break;
        case HealthMonitor::HealthStatus::UNHEALTHY:
            std::cout << "UNHEALTHY" << std::endl;
            break;
        case HealthMonitor::HealthStatus::CRITICAL:
            std::cout << "CRITICAL" << std::endl;
            break;
    }
    
    std::cout << "\n6. Checking module metrics..." << std::endl;
    auto calculatorMetrics = healthMonitor.getModuleMetrics("Calculator");
    auto unstableMetrics = healthMonitor.getModuleMetrics("UnstableModule");
    
    std::cout << "Calculator metrics:" << std::endl;
    std::cout << "  - Total loads: " << calculatorMetrics.totalLoads << std::endl;
    std::cout << "  - Average load time: " << calculatorMetrics.averageLoadTime.count() << "ms" << std::endl;
    
    std::cout << "UnstableModule metrics:" << std::endl;
    std::cout << "  - Total loads: " << unstableMetrics.totalLoads << std::endl;
    std::cout << "  - Failed operations: " << unstableMetrics.failedOperations << std::endl;
    
    std::cout << "\n7. Cleaning up..." << std::endl;
    manager.shutdown();
    healthMonitor.stopMonitoring();
    
    std::cout << "\n=== HEALTH MONITORING DEMO COMPLETED ===" << std::endl;
}

int main() {
    std::cout << "High-Availability Hot-Swap System - Health Monitoring Demo" << std::endl;
    std::cout << "=========================================================" << std::endl;
    
    try {
        demonstrateHealthMonitoring();
    } catch (const std::exception& e) {
        std::cerr << "Demo error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}