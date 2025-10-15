#pragma once
#include "../core/IModule.hpp"
#include <iostream>
#include <string>

class CalculatorModule : public IModule {
private:
    std::string name;
    std::string version;
    bool running;
    double lastResult;

public:
    CalculatorModule(const std::string& moduleName = "Calculator", 
                     const std::string& moduleVersion = "1.0")
        : name(moduleName), version(moduleVersion), running(false), lastResult(0.0) {
        std::cout << "CalculatorModule constructed: " << name << std::endl;
    }

    // VIRTUAL DESTRUCTOR ADD KARO - IMPORTANT!
    virtual ~CalculatorModule() {
        std::cout << "CalculatorModule destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "CalculatorModule initializing: " << name << std::endl;
        lastResult = 0.0;
        return true;
    }

    bool start() override {
        std::cout << "CalculatorModule starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "CalculatorModule stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "CalculatorModule cleaning: " << name << std::endl;
        lastResult = 0.0;
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
    
    double add(double a, double b) {
        lastResult = a + b;
        std::cout << "Calculator: " << a << " + " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double multiply(double a, double b) {
        lastResult = a * b;
        std::cout << "Calculator: " << a << " * " << b << " = " << lastResult << std::endl;
        return lastResult;
    }
    
    double getLastResult() const {
        return lastResult;
    }
};