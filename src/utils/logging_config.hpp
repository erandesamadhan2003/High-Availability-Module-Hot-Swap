#pragma once
#include "../src/utils/Logger.hpp"

class LoggingConfig {
public:
    static void setupProductionLogging() {
        auto& logger = Logger::getInstance();
        logger.setLogLevel(Logger::Level::INFO);
        logger.setLogFile("/var/log/hotswap_system.log");
        logger.enableConsoleOutput(false); // In production, log to file only
    }
    
    static void setupDevelopmentLogging() {
        auto& logger = Logger::getInstance();
        logger.setLogLevel(Logger::Level::DEBUG);
        logger.setLogFile("hotswap_system.log");
        logger.enableConsoleOutput(true); // In development, show on console
    }
    
    static void setupTestingLogging() {
        auto& logger = Logger::getInstance();
        logger.setLogLevel(Logger::Level::WARNING);
        logger.setLogFile("test_hotswap.log");
        logger.enableConsoleOutput(true);
    }
};