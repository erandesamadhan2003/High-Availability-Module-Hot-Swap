#include "CalculatorModuleV1.hpp"

extern "C" {
    IModule* createModule() {
        return new CalculatorModuleV1("Calculator", "1.0.0");
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
        return 1;
    }
}