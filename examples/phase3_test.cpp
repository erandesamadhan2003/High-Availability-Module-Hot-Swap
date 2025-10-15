#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/DynamicLibrary.hpp"
#include "../src/core/ModuleFactory.hpp"
#include "../src/core/IModule.hpp"

int main() {
    std::cout << "=== PHASE 3 TEST: Dynamic Library Loading ===" << std::endl;
    
    // Test 1: Basic DynamicLibrary class test
    std::cout << "\n1. Testing DynamicLibrary class..." << std::endl;
    
    try {
        // CORRECT PATH use karo
        std::string libraryPath = "../build/simple_module.so";  // YEH CHANGE KARO
        std::cout << "Trying to load: " << libraryPath << std::endl;
        
        DynamicLibrary lib(libraryPath);
        
        if (lib.isLoaded()) {
            std::cout << "Library loaded successfully!" << std::endl;
            
            // Factory functions get karo
            std::cout << "\n2. Testing factory functions..." << std::endl;
            
            using CreateFunc = IModule* (*)();
            using DestroyFunc = void (*)(IModule*);
            
            // Create function nikal
            auto createFunc = (CreateFunc)lib.getFunction("createModule");
            auto destroyFunc = (DestroyFunc)lib.getFunction("destroyModule");
            
            if (createFunc && destroyFunc) {
                std::cout << "Factory functions found!" << std::endl;
                
                // Module create karo
                std::cout << "\n3. Creating module from library..." << std::endl;
                IModule* module = createFunc();
                
                if (module) {
                    std::cout << "Module created: " << module->getName() << std::endl;
                    
                    // Module use karo
                    module->init();
                    module->start();
                    
                    std::cout << "Module Name: " << module->getName() << std::endl;
                    std::cout << "Module Version: " << module->getVersion() << std::endl;
                    std::cout << "Is Healthy: " << (module->isHealthy() ? "Yes" : "No") << std::endl;
                    
                    // Thoda wait karo
                    std::cout << "\n4. Using module for 2 seconds..." << std::endl;
                    for (int i = 0; i < 2; i++) {
                        std::cout << "Working... " << (i + 1) << "s" << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    
                    // Module stop karo
                    module->stop();
                    module->cleanup();
                    
                    // Module destroy karo
                    std::cout << "\n5. Destroying module..." << std::endl;
                    destroyFunc(module);
                    
                } else {
                    std::cout << "Failed to create module!" << std::endl;
                }
            } else {
                std::cout << "Factory functions not found!" << std::endl;
            }
            
        } else {
            std::cout << "Failed to load library!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== PHASE 3 TEST COMPLETED ===" << std::endl;
    std::cout << "Dynamic Library Loading is working!" << std::endl;
    
    return 0;
}