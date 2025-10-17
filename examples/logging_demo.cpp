#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/ModuleManager.hpp"
#include "../src/utils/Logger.hpp"

void demonstrateLogging() {
    auto& logger = Logger::getInstance();
    
    std::cout << "=== LOGGING SYSTEM DEMONSTRATION ===" << std::endl;
    
    // Test different log levels
    logger.debug("This is a debug message", "LoggingDemo");
    logger.info("This is an info message", "LoggingDemo");
    logger.warning("This is a warning message", "LoggingDemo");
    logger.error("This is an error message", "LoggingDemo");
    logger.critical("This is a critical message", "LoggingDemo");
    
    // Test without module name
    logger.info("This message has no module specified");
    
    std::cout << "\nAll log messages should appear above with different colors" << std::endl;
    std::cout << "Check hotswap_system.log file for persistent logs" << std::endl;
}

void demonstrateModuleManagerLogging() {
    auto& manager = ModuleManager::getInstance();
    auto& logger = Logger::getInstance();
    
    std::cout << "\n=== MODULE MANAGER LOGGING DEMONSTRATION ===" << std::endl;
    
    logger.info("Starting module operations", "ModuleDemo");
    
    // Load module with logging
    if (manager.loadModule("./simple_module.so")) {
        logger.info("Simple module loaded successfully", "ModuleDemo");
        
        // Use module for a while
        logger.debug("Using module for 2 seconds", "ModuleDemo");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Unload module with logging
        if (manager.unloadModule("SimpleModule")) {
            logger.info("Simple module unloaded successfully", "ModuleDemo");
        } else {
            logger.error("Failed to unload simple module", "ModuleDemo");
        }
    } else {
        logger.error("Failed to load simple module", "ModuleDemo");
    }
    
    logger.info("Module operations completed", "ModuleDemo");
}

void demonstrateLogLevels() {
    auto& logger = Logger::getInstance();
    
    std::cout << "\n=== LOG LEVEL DEMONSTRATION ===" << std::endl;
    
    // Show all levels
    std::cout << "Current log level: INFO (showing INFO and above)" << std::endl;
    logger.debug("This DEBUG message should NOT appear", "LogLevelDemo");
    logger.info("This INFO message should appear", "LogLevelDemo");
    logger.warning("This WARNING message should appear", "LogLevelDemo");
    
    // Change to DEBUG level
    std::cout << "\nChanging log level to DEBUG..." << std::endl;
    logger.setLogLevel(Logger::Level::DEBUG);
    logger.debug("Now this DEBUG message should appear", "LogLevelDemo");
    
    // Change to WARNING level
    std::cout << "\nChanging log level to WARNING..." << std::endl;
    logger.setLogLevel(Logger::Level::WARNING);
    logger.info("This INFO message should NOT appear", "LogLevelDemo");
    logger.warning("This WARNING message should appear", "LogLevelDemo");
    
    // Reset to INFO
    logger.setLogLevel(Logger::Level::INFO);
}

int main() {
    std::cout << "High-Availability Hot-Swap System - Logging Demo" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    try {
        demonstrateLogging();
        demonstrateModuleManagerLogging();
        demonstrateLogLevels();
        
        std::cout << "\n=== LOGGING DEMO COMPLETED ===" << std::endl;
        std::cout << "Check the generated log file: hotswap_system.log" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Demo error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}