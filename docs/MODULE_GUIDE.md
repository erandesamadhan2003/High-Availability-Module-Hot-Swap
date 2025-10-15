# Module Creation Guide

## Module Structure
Every module must:
1. Inherit from IModule
2. Implement all pure virtual functions
3. Provide factory functions with extern "C"

## Factory Functions Template
```cpp
extern "C" {
    IModule* createModule() {
        return new YourModuleClass("ModuleName", "1.0.0");
    }
    
    void destroyModule(IModule* module) {
        if (module) {
            delete module;
        }
    }
    
    const char* getModuleType() {
        return "YourModuleType";
    }
    
    int getModuleVersion() {
        return 1;
    }
}