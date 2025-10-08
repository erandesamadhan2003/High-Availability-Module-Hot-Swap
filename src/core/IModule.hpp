#pragma once 
#include <string>
#include <vector>

class IModule {
public: 
    virtual ~IModule() = default;

    virtual bool init() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool cleanup() = 0;

    virtual std::string getName() = 0;
    virtual std::string getVersion() = 0;

    virtual bool isHealthy() = 0;

    virtual std::vector<std::string> getDependencies() {
        return {}; // default empty dependencies
    };
};