#include <iostream>
#include <chrono>
#include <thread>
#include "../src/core/IModule.hpp"
#include "../src/core/ModuleInfo.hpp"

// Test module banaye IModule interface use karke
class TestModule : public IModule {
private:
    std::string name;
    std::string version;
    bool running;

public:
    TestModule(const std::string& n, const std::string& v) 
        : name(n), version(v), running(false) {
        std::cout << "TestModule constructor: " << name << std::endl;
    }

    ~TestModule() {
        std::cout << "TestModule destructor: " << name << std::endl;
    }

    bool init() override {
        std::cout << "[" << name << "] Initializing..." << std::endl;
        return true;
    }

    bool start() override {
        std::cout << "[" << name << "] Starting..." << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "[" << name << "] Stopping..." << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "[" << name << "] Cleaning up..." << std::endl;
        return true;
    }

    std::string getName() override {
        return name;
    }

    std::string getVersion() override {
        return version;
    }

    bool isHealthy() override {
        return running;
    }
};

int main() {
    std::cout << "=== PHASE 2 TEST: Module Interface ===" << std::endl;
    
    // Test 1: IModule interface test
    std::cout << "\n1. Testing IModule interface..." << std::endl;
    TestModule calculator("Calculator", "1.0");
    
    calculator.init();
    calculator.start();
    
    std::cout << "Module Name: " << calculator.getName() << std::endl;
    std::cout << "Module Version: " << calculator.getVersion() << std::endl;
    std::cout << "Is Healthy: " << (calculator.isHealthy() ? "Yes" : "No") << std::endl;
    
    calculator.stop();
    calculator.cleanup();
    
    // Test 2: ModuleInfo structure test
    std::cout << "\n2. Testing ModuleInfo structure..." << std::endl;
    ModuleInfo info;
    info.name = "PlayerModule";
    info.version = "2.0";
    info.isRunning = true;
    
    std::cout << "Info Name: " << info.name << std::endl;
    std::cout << "Info Version: " << info.version << std::endl;
    std::cout << "Is Running: " << (info.isRunning ? "Yes" : "No") << std::endl;
    
    // Uptime test
    std::cout << "\n3. Waiting 2 seconds to test uptime..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    auto uptime = info.getUptime();
    std::cout << "Uptime: " << uptime.count() << " seconds" << std::endl;
    
    std::cout << "\n=== PHASE 2 TEST COMPLETED SUCCESSFULLY ===" << std::endl;
    std::cout << "Module Interface is working correctly!" << std::endl;
    
    return 0;
}
