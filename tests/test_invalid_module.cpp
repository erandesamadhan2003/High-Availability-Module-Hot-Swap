#include <iostream>
#include <cassert>
#include "../src/core/ModuleManager.hpp"

void test_invalid_module() {
    std::cout << "Testing Invalid Module Handling..." << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    // Test loading non-existent module
    bool result = manager.loadModule("./non_existent_module.so");
    assert(!result && "Should fail to load non-existent module");
    std::cout << "✓ Non-existent module loading correctly failed" << std::endl;
    
    // Test loading invalid library file
    result = manager.loadModule("./CMakeLists.txt"); // Not a shared library
    assert(!result && "Should fail to load invalid library");
    std::cout << "✓ Invalid library loading correctly failed" << std::endl;
    
    std::cout << "Invalid Module Test: PASSED" << std::endl;
}

int main() {
    try {
        test_invalid_module();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test FAILED: " << e.what() << std::endl;
        return 1;
    }
}