#pragma once
#include <string>
#include <chrono>

struct ModuleInfo {
    std::string name;          
    std::string version;       
    std::string libraryPath;   
    
    
    bool isRunning = false;    
    bool isHealthy = true;    
    
    std::chrono::system_clock::time_point loadTime; 
    
    ModuleInfo() {
        loadTime = std::chrono::system_clock::now();
    }
    
    std::chrono::seconds getUptime() const {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - loadTime);
    }
};