#pragma once
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include "IModule.hpp"
#include "ModuleInfo.hpp"
#include "DynamicLibrary.hpp"

class ModuleManager {
private:
    static ModuleManager* instance;
    mutable std::mutex moduleMutex; // Thread safety ke liye

    // Module storage structure - jaise phone mein app info
    struct ModuleHandle {
        std::unique_ptr<DynamicLibrary> library; // Library handle
        IModule* module;                         // Module object
        ModuleInfo info;                         // Module information
        bool markedForUnload;                    // Safe unload ke liye
    };

    std::map<std::string, ModuleHandle> modules; // All modules store here

    // Private constructor - Singleton pattern
    ModuleManager() = default;
    ~ModuleManager() = default;

    // Helper functions
    bool safeModuleUnload(ModuleHandle& handle);
    void cleanupModuleResources(ModuleHandle& handle);

public:
    // Singleton pattern - prevent copying
    ModuleManager(const ModuleManager&) = delete;
    ModuleManager& operator=(const ModuleManager&) = delete;

    // Singleton access - ek hi instance hoga pure system mein
    static ModuleManager& getInstance();
    
    // === MAIN MODULE OPERATIONS ===
    
    // 1. Module load karna
    bool loadModule(const std::string& libraryPath);
    
    // 2. Module unload karna  
    bool unloadModule(const std::string& moduleName);
    
    // 3. Module reload karna (Hot-swap!)
    bool reloadModule(const std::string& moduleName);
    
    // 4. Module access karna
    IModule* getModule(const std::string& name);
    
    // 5. Module information
    ModuleInfo getModuleInfo(const std::string& name);
    
    // 6. All modules list karna
    std::vector<std::string> getAllModuleNames() const;
    
    // 7. Print all modules status
    void printAllModules() const;
    
    // 8. System cleanup - sab kuch band karna
    void shutdown();
    
    // 9. Check if module loaded hai
    bool isModuleLoaded(const std::string& moduleName) const;
    
    // 10. Get loaded modules count
    size_t getModuleCount() const;

    // Scan /proc/self/maps for loaded .so files and log them.
    // Compares runtime shared libs to modules managed by ModuleManager.
    void scanAndLogRuntimeSharedLibraries() const;
};