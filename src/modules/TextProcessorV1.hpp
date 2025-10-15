#pragma once
#include "../core/IModule.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

class TextProcessorV1 : public IModule {
private:
    std::string name;
    std::string version;
    bool running;
    int processCount;

public:
    TextProcessorV1(const std::string& moduleName = "TextProcessor", 
                    const std::string& moduleVersion = "1.0.0")
        : name(moduleName), version(moduleVersion), running(false), processCount(0) {
        std::cout << "TextProcessor V1 constructed: " << name << std::endl;
    }

    virtual ~TextProcessorV1() {
        std::cout << "TextProcessor V1 destroyed: " << name << std::endl;
    }

    bool init() override {
        std::cout << "TextProcessor V1 initializing: " << name << std::endl;
        processCount = 0;
        return true;
    }

    bool start() override {
        std::cout << "TextProcessor V1 starting: " << name << std::endl;
        running = true;
        return true;
    }

    bool stop() override {
        std::cout << "TextProcessor V1 stopping: " << name << std::endl;
        running = false;
        return true;
    }

    bool cleanup() override {
        std::cout << "TextProcessor V1 cleaning: " << name << std::endl;
        processCount = 0;
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
    
    // Text processing functions - Version 1
    std::string toUpperCase(const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        processCount++;
        std::cout << "TextProcessor V1: Converted to uppercase: " << result << std::endl;
        return result;
    }
    
    std::string toLowerCase(const std::string& text) {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        processCount++;
        std::cout << "TextProcessor V1: Converted to lowercase: " << result << std::endl;
        return result;
    }
    
    int countWords(const std::string& text) {
        int count = 0;
        bool inWord = false;
        
        for (char c : text) {
            if (std::isspace(c)) {
                inWord = false;
            } else if (!inWord) {
                count++;
                inWord = true;
            }
        }
        
        processCount++;
        std::cout << "TextProcessor V1: Word count: " << count << std::endl;
        return count;
    }
    
    int getProcessCount() const {
        return processCount;
    }
    
    void resetCounter() {
        processCount = 0;
        std::cout << "TextProcessor V1: Process counter reset" << std::endl;
    }
};