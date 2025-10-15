#pragma once
#include "../core/IModule.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

class CalculatorModuleV2 : public IModule {
private:
    std::string name;
    std::string version;
    bool running;
    double lastResult;
    int operationCount;
    std::vector<double> history;  // New in V2: operation history

public:
    CalculatorModuleV2(const std::string& moduleName = "Calculator", 
                       const std::string& moduleVersion = "2.0.0")
        : name(moduleName), version(moduleVersion), running(false), 
          lastResult(0.0), operationCount(0) {
        std::cout << "CalculatorModule V2 constructed: " << name << std::endl;
        history.reserve(100);  // Pre-allocate memory
    }

    virtual ~CalculatorModuleV2() {
        std::cout << "CalculatorModule V2 destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "CalculatorModule V2 initializing: " << name << std::endl;
        lastResult = 0.0;
        operationCount = 0;
        history.clear();
        return true;
    }

    bool start() override {
        std::cout << "CalculatorModule V2 starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "CalculatorModule V2 stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "CalculatorModule V2 cleaning: " << name << std::endl;
        lastResult = 0.0;
        operationCount = 0;
        history.clear();
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
    
    // Basic calculator functions (same as V1)
    double add(double a, double b) {
        lastResult = a + b;
        operationCount++;
        history.push_back(lastResult);
        std::cout << "Calculator V2: " << a << " + " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double subtract(double a, double b) {
        lastResult = a - b;
        operationCount++;
        history.push_back(lastResult);
        std::cout << "Calculator V2: " << a << " - " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double multiply(double a, double b) {
        lastResult = a * b;
        operationCount++;
        history.push_back(lastResult);
        std::cout << "Calculator V2: " << a << " * " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    // NEW IN V2: Advanced mathematical functions
    double divide(double a, double b) {
        if (b != 0) {
            lastResult = a / b;
            operationCount++;
            history.push_back(lastResult);
            std::cout << "Calculator V2: " << a << " / " << b << " = " << lastResult << std::endl;
            return lastResult;
        } else {
            std::cout << "Calculator V2: Error - Division by zero!" << std::endl;
            return 0.0;
        }
    }
    
    double power(double base, double exponent) {
        lastResult = std::pow(base, exponent);
        operationCount++;
        history.push_back(lastResult);
        std::cout << "Calculator V2: " << base << " ^ " << exponent << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double squareRoot(double value) {
        if (value >= 0) {
            lastResult = std::sqrt(value);
            operationCount++;
            history.push_back(lastResult);
            std::cout << "Calculator V2: sqrt(" << value << ") = " << lastResult << std::endl;
            return lastResult;
        } else {
            std::cout << "Calculator V2: Error - Square root of negative number!" << std::endl;
            return 0.0;
        }
    }
    
    // NEW IN V2: History features
    void printHistory() const {
        std::cout << "Calculator V2 - Operation History (" << history.size() << " operations):" << std::endl;
        for (size_t i = 0; i < history.size(); ++i) {
            std::cout << "  " << (i + 1) << ": " << history[i] << std::endl;
        }
    }
    
    void clearHistory() {
        history.clear();
        std::cout << "Calculator V2: History cleared" << std::endl;
    }
    
    size_t getHistorySize() const {
        return history.size();
    }
    
    // Common functions
    int getOperationCount() const {
        return operationCount;
    }
    
    double getLastResult() const {
        return lastResult;
    }
    
    void resetCounter() {
        operationCount = 0;
        std::cout << "Calculator V2: Operation counter reset" << std::endl;
    }
};