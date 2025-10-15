#pragma once

class IModule;

extern "C" {
    IModule* createModule();
    
    void destroyModule(IModule* module);
    
    const char* getModuleType();
    
    int getModuleVersion();
}