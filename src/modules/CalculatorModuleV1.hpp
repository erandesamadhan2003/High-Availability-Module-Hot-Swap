#pragma once
#include "../core/IModule.hpp"
#include <iostream>
#include <string>

class CalculatorModuleV1 : public IModule {
private:
    std::string name;
    std::string version;
    bool running;
    double lastResult;
    int operationCount;

public:
    CalculatorModuleV1(const std::string& moduleName = "Calculator", 
                       const std::string& moduleVersion = "1.0")
        : name(moduleName), version(moduleVersion), running(false), 
          lastResult(0.0), operationCount(0) {
        std::cout << "CalculatorModule V1 constructed: " << name << std::endl;
    }

    virtual ~CalculatorModuleV1() {
        std::cout << "CalculatorModule V1 destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "CalculatorModule V1 initializing: " << name << std::endl;
        lastResult = 0.0;
        operationCount = 0;
        return true;
    }

    bool start() override {
        std::cout << "CalculatorModule V1 starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "CalculatorModule V1 stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "CalculatorModule V1 cleaning: " << name << std::endl;
        lastResult = 0.0;
        operationCount = 0;
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
    
    // Basic calculator functions - Version 1 features
    double add(double a, double b) {
        lastResult = a + b;
        operationCount++;
        std::cout << "Calculator V1: " << a << " + " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double subtract(double a, double b) {
        lastResult = a - b;
        operationCount++;
        std::cout << "Calculator V1: " << a << " - " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double multiply(double a, double b) {
        lastResult = a * b;
        operationCount++;
        std::cout << "Calculator V1: " << a << " * " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    // Version 1 specific function
    int getOperationCount() const {
        return operationCount;
    }
    
    double getLastResult() const {
        return lastResult;
    }
    
    void resetCounter() {
        operationCount = 0;
        std::cout << "Calculator V1: Operation counter reset" << std::endl;
    }
};