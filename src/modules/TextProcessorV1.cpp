#include "TextProcessorV1.hpp"

extern "C" {
    IModule* createModule() {
        return new TextProcessorV1("TextProcessor", "1.0.0");
    }
    
    void destroyModule(IModule* module) {
        if (module) {
            delete module;
        }
    }
    
    const char* getModuleType() {
        return "TextProcessor";
    }
    
    int getModuleVersion() {
        return 1;
    }
}