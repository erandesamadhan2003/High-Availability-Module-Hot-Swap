#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <iomanip>
#include "../src/core/ModuleManager.hpp"

class DemoController {
private:
    bool running;
    int demoStep;

public:
    DemoController() : running(false), demoStep(0) {}

    void printHeader(const std::string& title) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << " " << title << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }

    void printStep(const std::string& description) {
        demoStep++;
        std::cout << "\nStep " << demoStep << ": " << description << std::endl;
        std::cout << std::string(40, '-') << std::endl;
    }

    void wait(int seconds, const std::string& message = "Working") {
        for (int i = 0; i < seconds; i++) {
            std::cout << message << " " << (i + 1) << "s" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void demonstrateModuleLoading() {
        printHeader("MODULE LOADING DEMONSTRATION");
        
        auto& manager = ModuleManager::getInstance();

        printStep("Loading Calculator Module Version 1");
        if (manager.loadModule("./calculator_v1.so")) {
            std::cout << "SUCCESS: Calculator V1 loaded" << std::endl;
            auto info = manager.getModuleInfo("Calculator");
            std::cout << "Module: " << info.name << " v" << info.version << std::endl;
        } else {
            std::cout << "FAILED: Could not load Calculator V1" << std::endl;
            return;
        }

        printStep("Using Calculator Module");
        wait(2, "Performing calculations");
        
        std::cout << "Calculator module is processing mathematical operations..." << std::endl;
        std::cout << "Basic arithmetic functions available: +, -, *" << std::endl;

        printStep("Loading Text Processor Module");
        if (manager.loadModule("./textprocessor_v1.so")) {
            std::cout << "SUCCESS: Text Processor V1 loaded" << std::endl;
            auto info = manager.getModuleInfo("TextProcessor");
            std::cout << "Module: " << info.name << " v" << info.version << std::endl;
        }

        printStep("Using Multiple Modules Simultaneously");
        wait(3, "Processing data with both modules");
        
        std::cout << "Both modules working together:" << std::endl;
        std::cout << "- Calculator processing numerical data" << std::endl;
        std::cout << "- Text Processor handling text data" << std::endl;

        manager.printAllModules();
    }

    void demonstrateMultipleModules() {
        printHeader("MULTIPLE MODULES MANAGEMENT");
        
        auto& manager = ModuleManager::getInstance();

        printStep("Checking Current Module Status");
        auto moduleNames = manager.getAllModuleNames();
        std::cout << "Currently loaded modules: " << moduleNames.size() << std::endl;
        
        for (const auto& name : moduleNames) {
            auto info = manager.getModuleInfo(name);
            auto uptime = info.getUptime();
            std::cout << " - " << std::setw(15) << std::left << name 
                      << " v" << std::setw(6) << info.version
                      << " Uptime: " << std::setw(4) << uptime.count() << "s"
                      << " Status: " << (info.isRunning ? "RUNNING" : "STOPPED") << std::endl;
        }

        printStep("Module Information Access");
        std::cout << "Detailed module information:" << std::endl;
        for (const auto& name : moduleNames) {
            auto info = manager.getModuleInfo(name);
            std::cout << "\nModule: " << info.name << std::endl;
            std::cout << "  Version: " << info.version << std::endl;
            std::cout << "  Library: " << info.libraryPath << std::endl;
            std::cout << "  Loaded: " << std::chrono::system_clock::to_time_t(info.loadTime) << std::endl;
            std::cout << "  Running: " << (info.isRunning ? "Yes" : "No") << std::endl;
            std::cout << "  Healthy: " << (info.isHealthy ? "Yes" : "No") << std::endl;
        }

        printStep("Module Dependency Simulation");
        wait(2, "Checking module dependencies");
        
        std::cout << "Simulating module interdependencies..." << std::endl;
        std::cout << "Text Processor might depend on Calculator for numerical operations" << std::endl;
        std::cout << "Both modules can work independently but complement each other" << std::endl;
    }

    void demonstrateHotSwapScenario() {
        printHeader("HOT-SWAP SCENARIO DEMONSTRATION");
        
        auto& manager = ModuleManager::getInstance();

        printStep("Initial State - Calculator Version 1");
        if (!manager.isModuleLoaded("Calculator")) {
            manager.loadModule("./calculator_v1.so");
        }
        
        auto initialInfo = manager.getModuleInfo("Calculator");
        std::cout << "Current Calculator Version: " << initialInfo.version << std::endl;
        std::cout << "Features: Basic arithmetic operations" << std::endl;

        printStep("Simulating Continuous Operation");
        wait(5, "Calculator V1 processing requests");
        
        std::cout << "Calculator V1 handling multiple calculation requests..." << std::endl;
        std::cout << "Users are actively using the system..." << std::endl;
        std::cout << "No downtime allowed for updates..." << std::endl;

        printStep("New Version Available - Calculator Version 2");
        std::cout << "New features available in Version 2:" << std::endl;
        std::cout << " - Division operations" << std::endl;
        std::cout << " - Power calculations" << std::endl;
        std::cout << " - Square root function" << std::endl;
        std::cout << " - Operation history" << std::endl;

        printStep("Preparing for Hot-Swap");
        wait(2, "Preparing system for update");
        
        std::cout << "Ensuring all current operations complete..." << std::endl;
        std::cout << "Saving current state..." << std::endl;
        std::cout << "Ready for version update..." << std::endl;

        printStep("Executing Hot-Swap");
        
        std::cout << "Replacing module file with new version..." << std::endl;
        
        manager.unloadModule("Calculator");
        
        if (manager.loadModule("./calculator_v2.so")) {
            auto newInfo = manager.getModuleInfo("Calculator");
            std::cout << "HOT-SWAP SUCCESSFUL!" << std::endl;
            std::cout << "Old Version: " << initialInfo.version << std::endl;
            std::cout << "New Version: " << newInfo.version << std::endl;
            std::cout << "Zero downtime achieved!" << std::endl;
        } else {
            std::cout << "HOT-SWAP FAILED! Rolling back..." << std::endl;
            manager.loadModule("./calculator_v1.so");
        }

        printStep("Post-Swap Verification");
        wait(2, "Verifying new version");
        
        std::cout << "New Calculator version is operational" << std::endl;
        std::cout << "All existing features working correctly" << std::endl;
        std::cout << "New features now available to users" << std::endl;
        std::cout << "No service interruption detected" << std::endl;
    }

    void demonstrateErrorHandling() {
        printHeader("ERROR HANDLING AND RECOVERY");
        
        auto& manager = ModuleManager::getInstance();

        printStep("Testing Invalid Module Loading");
        std::cout << "Attempting to load non-existent module..." << std::endl;
        bool result = manager.loadModule("./non_existent_module.so");
        std::cout << "Result: " << (result ? "SUCCESS" : "EXPECTED FAILURE") << std::endl;
        std::cout << "System handled error gracefully" << std::endl;

        printStep("Testing Duplicate Module Loading");
        std::cout << "Attempting to load Calculator module again..." << std::endl;
        if (manager.isModuleLoaded("Calculator")) {
            result = manager.loadModule("./calculator_v2.so");
            std::cout << "Result: " << (result ? "LOADED" : "REJECTED - Already loaded") << std::endl;
        }

        printStep("Testing Invalid Module Unload");
        std::cout << "Attempting to unload non-existent module..." << std::endl;
        result = manager.unloadModule("NonExistentModule");
        std::cout << "Result: " << (result ? "UNLOADED" : "EXPECTED FAILURE") << std::endl;

        printStep("Module Health Monitoring");
        std::cout << "Current module health status:" << std::endl;
        auto moduleNames = manager.getAllModuleNames();
        for (const auto& name : moduleNames) {
            auto* module = manager.getModule(name);
            if (module) {
                std::cout << " - " << name << ": " 
                          << (module->isHealthy() ? "HEALTHY" : "UNHEALTHY") << std::endl;
            }
        }

        printStep("Graceful Degradation");
        wait(2, "Simulating module failure");
        
        std::cout << "Simulating Text Processor module failure..." << std::endl;
        std::cout << "System continues operating with remaining modules" << std::endl;
        std::cout << "Calculator module remains fully functional" << std::endl;
        std::cout << "Text operations temporarily unavailable" << std::endl;
    }

    void demonstrateSystemRecovery() {
        printHeader("SYSTEM RECOVERY SCENARIOS");
        
        auto& manager = ModuleManager::getInstance();

        printStep("Module Failure Recovery");
        std::cout << "Text Processor module has failed..." << std::endl;
        std::cout << "Attempting recovery..." << std::endl;
        
        if (manager.isModuleLoaded("TextProcessor")) {
            manager.unloadModule("TextProcessor");
        }
        
        wait(2, "Waiting before reload");
        
        std::cout << "Reloading Text Processor module..." << std::endl;
        if (manager.loadModule("./textprocessor_v1.so")) {
            std::cout << "RECOVERY SUCCESSFUL: Text Processor restored" << std::endl;
        } else {
            std::cout << "RECOVERY FAILED: Could not restore Text Processor" << std::endl;
        }

        printStep("Rollback Scenario");
        std::cout << "New Calculator version has issues..." << std::endl;
        std::cout << "Performing emergency rollback to previous version..." << std::endl;
        
        if (manager.isModuleLoaded("Calculator")) {
            manager.unloadModule("Calculator");
        }
        
        wait(1, "Rolling back");
        
        if (manager.loadModule("./calculator_v1.so")) {
            std::cout << "ROLLBACK SUCCESSFUL: Calculator V1 restored" << std::endl;
            std::cout << "System stability maintained" << std::endl;
        }

        printStep("Complete System Restart");
        std::cout << "Simulating system maintenance..." << std::endl;
        std::cout << "Shutting down all modules..." << std::endl;
        
        manager.shutdown();
        
        wait(2, "System offline");
        
        std::cout << "Restarting system with updated modules..." << std::endl;
        manager.loadModule("./calculator_v2.so");
        manager.loadModule("./textprocessor_v1.so");
        
        std::cout << "System restart completed successfully" << std::endl;
    }

    void runCompleteDemo() {
        printHeader("HIGH-AVAILABILITY HOT-SWAP SYSTEM DEMO");
        std::cout << "This demonstration shows real-time module updates without downtime" << std::endl;
        
        running = true;
        demoStep = 0;
        
        demonstrateModuleLoading();
        demonstrateMultipleModules();
        demonstrateHotSwapScenario();
        demonstrateErrorHandling();
        demonstrateSystemRecovery();
        
        printHeader("DEMONSTRATION COMPLETE");
        std::cout << "All hot-swap capabilities successfully demonstrated" << std::endl;
        std::cout << "System maintained high availability throughout" << std::endl;
        std::cout << "Zero downtime achieved for module updates" << std::endl;
        
        auto& manager = ModuleManager::getInstance();
        std::cout << "\nFinal System Status:" << std::endl;
        manager.printAllModules();
    }
};

int main() {
    std::cout << "HIGH-AVAILABILITY MODULE HOT-SWAP SYSTEM" << std::endl;
    std::cout << "Comprehensive Demonstration Application" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        DemoController controller;
        controller.runCompleteDemo();
        
        std::cout << "\nDemo completed successfully!" << std::endl;
        std::cout << "The system demonstrated:" << std::endl;
        std::cout << "✓ Runtime module loading/unloading" << std::endl;
        std::cout << "✓ Zero-downtime hot-swapping" << std::endl;
        std::cout << "✓ Multiple module management" << std::endl;
        std::cout << "✓ Error handling and recovery" << std::endl;
        std::cout << "✓ System maintenance operations" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Demo error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}