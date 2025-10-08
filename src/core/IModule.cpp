#include "IModule.hpp"
#include <iostream>



/*
IModule USAGE EXAMPLE:

class MyModule : public IModule {
public:
    bool init() override {
        std::cout << "MyModule initializing..." << std::endl;
        return true;
    }
    
    void start() override {
        std::cout << "MyModule starting..." << std::endl;
    }
    
    void stop() override {
        std::cout << "MyModule stopping..." << std::endl;
    }
    
    void cleanup() override {
        std::cout << "MyModule cleaning up..." << std::endl;
    }
    
    std::string getName() override {
        return "MyModule";
    }
    
    std::string getVersion() override {
        return "1.0";
    }
    
    bool isHealthy() override {
        return true;
    }
};
*/