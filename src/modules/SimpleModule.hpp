#pragma once
#include "../core/IModule.hpp"
#include <iostream>

class SimpleModule : public IModule {
private:
    std::string name;
    std::string version;
    bool running;

public:
    SimpleModule(const std::string& moduleName = "SimpleModule", 
                 const std::string& moduleVersion = "1.0")
        : name(moduleName), version(moduleVersion), running(false) {
        std::cout << "🔨 SimpleModule constructed: " << name << std::endl;
    }

    ~SimpleModule() {
        std::cout << "🗑️ SimpleModule destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "SimpleModule initializing: " << name << std::endl;
        return true;
    }

    bool start() override {
        std::cout << "SimpleModule starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "SimpleModule stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "SimpleModule cleaning: " << name << std::endl;
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
    
    // Module-specific function
    void sayHello() {
        std::cout << "Hello from " << name << " v" << version << std::endl;
    }
};