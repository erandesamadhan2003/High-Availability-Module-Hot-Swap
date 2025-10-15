#include <iostream>
#include <cassert>
#include "../src/core/ModuleManager.hpp"

void test_basic_loading() {
    std::cout << "Testing Basic Module Loading..." << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    
    // Test loading simple module
    bool result = manager.loadModule("./simple_module.so");
    assert(result && "Failed to load simple_module");
    std::cout << "✓ simple_module loaded successfully" << std::endl;
    
    // Test module information
    auto info = manager.getModuleInfo("SimpleModule");
    assert(info.name == "SimpleModule" && "Wrong module name");
    assert(info.version == "1.0" && "Wrong module version");
    std::cout << "✓ Module information correct" << std::endl;
    
    // Test module access
    auto* module = manager.getModule("SimpleModule");
    assert(module != nullptr && "Failed to get module");
    assert(module->isHealthy() && "Module not healthy");
    std::cout << "✓ Module access and health check passed" << std::endl;
    
    // Test unloading
    result = manager.unloadModule("SimpleModule");
    assert(result && "Failed to unload module");
    std::cout << "✓ Module unloaded successfully" << std::endl;
    
    std::cout << "Basic Loading Test: PASSED" << std::endl;
}

int main() {
    try {
        test_basic_loading();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test FAILED: " << e.what() << std::endl;
        return 1;
    }
}