#include "CalculatorModuleV2.hpp"

extern "C" {
    IModule* createModule() {
        return new CalculatorModuleV2("Calculator", "2.0.0");
    }
    
    void destroyModule(IModule* module) {
        if (module) {
            delete module;
        }
    }
    
    const char* getModuleType() {
        return "Calculator";
    }
    
    int getModuleVersion() {
        return 2;
    }
}