#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/ModuleManager.hpp"

int main() {
    std::cout << "Simple Hot-Swap Demo" << std::endl;
    std::cout << "====================" << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    std::cout << "\n1. Loading Calculator V1..." << std::endl;
    manager.loadModule("./calculator_v1.so");
    manager.printAllModules();
    
    std::cout << "\n2. Using system for 3 seconds..." << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "Working... " << (i + 1) << "s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\n3. Hot-swapping to Calculator V2..." << std::endl;
    manager.unloadModule("Calculator");
    manager.loadModule("./calculator_v2.so");
    manager.printAllModules();
    
    std::cout << "\n4. Using new version for 3 seconds..." << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << "Working with new features... " << (i + 1) << "s" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\n5. Cleaning up..." << std::endl;
    manager.shutdown();
    
    std::cout << "\nDemo completed successfully!" << std::endl;
    return 0;
}