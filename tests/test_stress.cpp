#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>
#include "../src/core/ModuleManager.hpp"

void stress_test() {
    std::cout << "Running Stress Test..." << std::endl;
    
    auto& manager = ModuleManager::getInstance();
    const int NUM_ITERATIONS = 10;
    const int NUM_THREADS = 3;
    
    std::vector<std::thread> threads;
    
    // Test function for each thread
    auto test_function = [&](int thread_id) {
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // Load module
            bool loaded = manager.loadModule("./simple_module.so");
            if (loaded) {
                // Access module
                auto* module = manager.getModule("SimpleModule");
                if (module) {
                    // Simulate work
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                
                // Unload module
                manager.unloadModule("SimpleModule");
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    };
    
    // Start multiple threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(test_function, i);
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Stress Test: PASSED - No crashes during concurrent access" << std::endl;
}

int main() {
    try {
        stress_test();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Stress Test FAILED: " << e.what() << std::endl;
        return 1;
    }
}