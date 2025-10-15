#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/ModuleManager.hpp"

int main() {
    std::cout << "=== PHASE 4 TEST FIXED: Module Manager ===" << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    try {
        // Test 1: Multiple modules load karo
        std::cout << "\n1. Loading multiple modules..." << std::endl;
        
        manager.loadModule("./simple_module.so");
        manager.loadModule("./calculator_v1.so");
        
        manager.printAllModules();
        
        // Test 2: Module access test
        std::cout << "\n2. Testing module access..." << std::endl;
        
        auto moduleNames = manager.getAllModuleNames();
        std::cout << "Loaded modules: " << moduleNames.size() << std::endl;
        for (const auto& name : moduleNames) {
            std::cout << " - " << name << std::endl;
        }
        
        // Test 3: Clean shutdown
        std::cout << "\n3. Testing clean shutdown..." << std::endl;
        manager.shutdown();
        
        std::cout << "\n=== PHASE 4 TEST COMPLETED SUCCESSFULLY ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}