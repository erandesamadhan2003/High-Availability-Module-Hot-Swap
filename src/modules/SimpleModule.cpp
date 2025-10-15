#include "SimpleModule.hpp"


extern "C" {
    IModule* createModule() {
        return new SimpleModule("SimpleModule", "1.0");
    }
    
    void destroyModule(IModule* module) {
        if (module) {
            delete module;
        }
    }
    
    const char* getModuleType() {
        return "Simple";
    }
    
    int getModuleVersion() {
        return 1;
    }
}