#include "CalculatorModule.hpp"

extern "C" {
    IModule* createModule() {
        return new CalculatorModule("Calculator", "1.0");
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