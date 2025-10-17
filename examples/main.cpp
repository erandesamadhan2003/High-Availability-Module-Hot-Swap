#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/ModuleManager.hpp"

int main() {
    std::cout << "=== Hot-Swap System Demo ===" << std::endl;
    std::cout << "Phase 1: Basic Setup Test" << std::endl;
    
    // ModuleManager test karo
    auto& manager = ModuleManager::getInstance();
    manager.scanAndLogRuntimeSharedLibraries();
    
    std::cout << "\n1. Testing ModuleManager singleton..." << std::endl;
    manager.printAllModules();
    
    std::cout << "\n2. Testing module loading (stub)..." << std::endl;
    if (manager.loadModule("./test_module.so")) {
        std::cout << "Module loading initiated successfully!" << std::endl;
    } else {
        std::cout << "Module loading failed!" << std::endl;
    }
    
    std::cout << "\n3. Waiting for 2 seconds..." << std::endl;
    for (int i = 0; i < 2; i++) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\n\n=== Phase 1 Test Completed Successfully ===" << std::endl;
    std::cout << "Project structure and build system is working!" << std::endl;
    
    return 0;
}