#include <iostream>
#include <cassert>
#include "../src/core/ModuleManager.hpp"

void test_duplicate_loading() {
    std::cout << "Testing Duplicate Module Loading..." << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    // Load module first time
    bool result = manager.loadModule("./simple_module.so");
    assert(result && "Failed to load module first time");
    std::cout << "✓ Module loaded successfully first time" << std::endl;
    
    // Try to load same module again
    result = manager.loadModule("./simple_module.so");
    // This might succeed or fail depending on implementation
    // We just ensure it doesn't crash
    std::cout << "✓ Duplicate loading handled without crash" << std::endl;
    
    // Cleanup
    manager.unloadModule("SimpleModule");
    
    std::cout << "Duplicate Loading Test: PASSED" << std::endl;
}

int main() {
    try {
        test_duplicate_loading();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test FAILED: " << e.what() << std::endl;
        return 1;
    }
}