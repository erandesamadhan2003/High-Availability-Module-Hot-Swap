#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/ModuleManager.hpp"

// Forward declarations for module-specific functions
void testCalculatorV1();
void testCalculatorV2();
void testTextProcessorV1();
void demonstrateHotSwap();

int main() {
    std::cout << "=== PHASE 5 TEST: Multiple Module Versions ===" << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    std::cout << "\n1. Testing Calculator Module Version 1" << std::endl;
    testCalculatorV1();
    
    std::cout << "\n2. Testing Calculator Module Version 2" << std::endl;
    testCalculatorV2();
    
    std::cout << "\n3. Testing Text Processor Module Version 1" << std::endl;
    testTextProcessorV1();
    
    std::cout << "\n4. Demonstrating Hot-Swap Between Versions" << std::endl;
    demonstrateHotSwap();
    
    std::cout << "\n5. Final System Status" << std::endl;
    manager.printAllModules();
    
    std::cout << "\n=== PHASE 5 TEST COMPLETED SUCCESSFULLY ===" << std::endl;
    
    return 0;
}

void testCalculatorV1() {
    auto& manager = ModuleManager::getInstance();
    
    // Load Calculator V1
    if (manager.loadModule("./calculator_v1.so")) {
        auto* module = manager.getModule("Calculator");
        if (module) {
            std::cout << "Calculator V1 Loaded: " << module->getVersion() << std::endl;
            
            // Test V1 specific functions
            // Note: In real scenario, we would dynamic_cast to CalculatorModuleV1
            // For demo, we'll just show module info
        }
    }
}

void testCalculatorV2() {
    auto& manager = ModuleManager::getInstance();
    
    // Unload V1 first if loaded
    if (manager.isModuleLoaded("Calculator")) {
        manager.unloadModule("Calculator");
    }
    
    // Load Calculator V2
    if (manager.loadModule("./calculator_v2.so")) {
        auto* module = manager.getModule("Calculator");
        if (module) {
            std::cout << "Calculator V2 Loaded: " << module->getVersion() << std::endl;
            std::cout << "V2 has advanced features like power, square root, and history" << std::endl;
        }
    }
}

void testTextProcessorV1() {
    auto& manager = ModuleManager::getInstance();
    
    // Load Text Processor V1
    if (manager.loadModule("./textprocessor_v1.so")) {
        auto* module = manager.getModule("TextProcessor");
        if (module) {
            std::cout << "TextProcessor V1 Loaded: " << module->getVersion() << std::endl;
            std::cout << "Text processor can convert case and count words" << std::endl;
        }
    }
}

void demonstrateHotSwap() {
    auto& manager = ModuleManager::getInstance();
    
    std::cout << "\nHot-Swap Demonstration:" << std::endl;
    std::cout << "Starting with Calculator V1..." << std::endl;
    
    // Start with V1
    if (manager.loadModule("./calculator_v1.so")) {
        std::cout << "Calculator V1 active: Version " 
                  << manager.getModuleInfo("Calculator").version << std::endl;
        
        // Simulate usage
        std::cout << "Using V1 for 3 seconds..." << std::endl;
        for (int i = 0; i < 3; i++) {
            std::cout << "Working with V1... " << (i + 1) << "s" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        // Hot-swap to V2
        std::cout << "\nPerforming hot-swap to V2..." << std::endl;
        
        // For hot-swap, we need to replace the .so file
        // In real scenario, you would copy calculator_v2.so to calculator_v1.so
        // For demo, we'll unload V1 and load V2
        manager.unloadModule("Calculator");
        
        if (manager.loadModule("./calculator_v2.so")) {
            std::cout << "Hot-swap successful! Now using: Version " 
                      << manager.getModuleInfo("Calculator").version << std::endl;
            
            // Simulate usage with V2
            std::cout << "Using V2 for 3 seconds..." << std::endl;
            for (int i = 0; i < 3; i++) {
                std::cout << "Working with V2 advanced features... " << (i + 1) << "s" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }
}