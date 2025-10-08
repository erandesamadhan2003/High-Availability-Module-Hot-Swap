#pragma once
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "IModule.hpp"
#include "ModuleInfo.hpp"
#include "DynamicLibrary.hpp"

// Forward declarations
class IModule;
class DynamicLibrary;

class ModuleManager {
private:
    static ModuleManager* instance;
    std::mutex moduleMutex;

    struct ModulehHandle {
        std::unique_ptr<DynamicLibrary> library;
        IModule* module;
        ModuleInfo info;
    };

    std::map<std::string, ModulehHandle> modules;

    ModuleManager() = default;
    ~ModuleManager() = default;

public:
    // Singleton pattern - prevent copying
    ModuleManager(const ModuleManager&) = delete;
    ModuleManager& operator=(const ModuleManager&) = delete;

    static ModuleManager& getInstance();
    
    bool loadModule(const std::string& libraryPath);
    bool unloadModule(const std::string& moduleName);
    bool reloadModule(const std::string& moduleName);

    IModule* getModule(const std::string& moduleName);
    ModuleInfo getModuleInfo(const std::string& moduleName);
    
    void printAllModules();
};