#include "ModuleManager.hpp"
#include <iostream>
#include <dlfcn.h>

// Singleton instance
ModuleManager* ModuleManager::instance = nullptr;

// Singleton access
ModuleManager& ModuleManager::getInstance() {
    if (!instance) {
        instance = new ModuleManager();
        std::cout << "ModuleManager instance created" << std::endl;
    }
    return *instance;
}

// Module load karna - MOST IMPORTANT FUNCTION
bool ModuleManager::loadModule(const std::string& libraryPath) {
    std::lock_guard<std::mutex> lock(moduleMutex); // Thread safety
    
    std::cout << "\nMODULE LOAD START: " << libraryPath << std::endl;

    try {
        // Step 1: Library load karo
        auto library = std::make_unique<DynamicLibrary>(libraryPath);
        if (!library->isLoaded()) {
            std::cerr << "Failed to load library: " << libraryPath << std::endl;
            return false;
        }

        // Step 2: Factory functions get karo
        using CreateFunc = IModule* (*)();
        auto createModule = (CreateFunc)library->getFunction("createModule");
        auto destroyModule = (void (*)(IModule*))library->getFunction("destroyModule");

        if (!createModule || !destroyModule) {
            std::cerr << "Factory functions not found in: " << libraryPath << std::endl;
            return false;
        }

        // Step 3: Module create karo
        IModule* module = createModule();
        if (!module) {
            std::cerr << "Failed to create module from: " << libraryPath << std::endl;
            return false;
        }

        // Step 4: Module initialize karo
        if (!module->init()) {
            std::cerr << "Module initialization failed: " << libraryPath << std::endl;
            destroyModule(module);
            return false;
        }

        // Step 5: ModuleInfo setup karo
        ModuleInfo info;
        info.name = module->getName();
        info.version = module->getVersion();
        info.libraryPath = libraryPath;
        info.loadTime = std::chrono::system_clock::now();

        // Step 6: ModuleHandle create karo
        ModuleHandle handle;
        handle.library = std::move(library);
        handle.module = module;
        handle.info = info;
        handle.markedForUnload = false;

        // Custom deleter with destroy function
        struct ModuleDeleter {
            void (*destroy)(IModule*);
            ModuleDeleter(void (*d)(IModule*)) : destroy(d) {}
            void operator()(IModule* mod) const {
                if (mod && destroy) {
                    mod->stop();
                    mod->cleanup();
                    destroy(mod);
                }
            }
        };

        modules[info.name] = std::move(handle);

        module->start();
        modules[info.name].info.isRunning = true;
        modules[info.name].info.isHealthy = true;

        std::cout << "MODULE LOAD SUCCESS: " << info.name 
                  << " v" << info.version << std::endl;
                  
        return true;

    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION in loadModule: " << e.what() << std::endl;
        return false;
    }
}

bool ModuleManager::unloadModule(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto it = modules.find(moduleName);
    if (it == modules.end()) {
        std::cerr << "Module not found: " << moduleName << std::endl;
        return false;
    }

    std::cout << "\nMODULE UNLOAD START: " << moduleName << std::endl;

    try {
        ModuleHandle& handle = it->second;
        
        // Step 1: Module stop karo
        if (handle.module) {
            handle.module->stop();
            handle.info.isRunning = false;
        }

        // Step 2: Cleanup karo
        cleanupModuleResources(handle);

        // Step 3: Map se remove karo
        modules.erase(it);
        
        std::cout << "MODULE UNLOAD SUCCESS: " << moduleName << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION in unloadModule: " << e.what() << std::endl;
        return false;
    }
}

// Helper: Module resources cleanup
void ModuleManager::cleanupModuleResources(ModuleHandle& handle) {
    if (handle.module) {
        handle.module->cleanup();
        
        // Factory destroy function use karo
        using DestroyFunc = void (*)(IModule*);
        auto destroyModule = (DestroyFunc)handle.library->getFunction("destroyModule");
        
        if (destroyModule) {
            destroyModule(handle.module);
        } else {
            delete handle.module; // Fallback
        }
        
        handle.module = nullptr;
    }
}

// Safe module unload - extra safety
bool ModuleManager::safeModuleUnload(ModuleHandle& handle) {
    if (handle.markedForUnload) {
        std::cout << "Module already marked for unload: " << handle.info.name << std::endl;
        return false;
    }
    
    handle.markedForUnload = true;
    return true;
}

// Module access karna
IModule* ModuleManager::getModule(const std::string& name) {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto it = modules.find(name);
    if (it != modules.end()) {
        return it->second.module;
    }
    
    std::cerr << "Module not found: " << name << std::endl;
    return nullptr;
}

// Module information get karna
ModuleInfo ModuleManager::getModuleInfo(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto it = modules.find(moduleName);
    if (it != modules.end()) {
        return it->second.info;
    }
    
    // Return empty info if not found
    return ModuleInfo{};
}

// All module names get karna
std::vector<std::string> ModuleManager::getAllModuleNames() const {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    std::vector<std::string> names;
    for (const auto& pair : modules) {
        names.push_back(pair.first);
    }
    
    return names;
}

// Module loaded hai ya nahi check karna
bool ModuleManager::isModuleLoaded(const std::string& moduleName) const {
    std::lock_guard<std::mutex> lock(moduleMutex);
    return modules.find(moduleName) != modules.end();
}

// Total modules count
size_t ModuleManager::getModuleCount() const {
    std::lock_guard<std::mutex> lock(moduleMutex);
    return modules.size();
}


// MOST IMPORTANT: HOT-SWAP FUNCTION
bool ModuleManager::reloadModule(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto it = modules.find(moduleName);
    if (it == modules.end()) {
        std::cerr << "Module not found for reload: " << moduleName << std::endl;
        return false;
    }

    std::cout << "\nHOT-SWAP STARTED: " << moduleName << std::endl;
    
    std::string libraryPath = it->second.info.libraryPath;
    
    try {
        // Step 1: Old module unload karo
        std::cout << "Step 1: Unloading old module..." << std::endl;
        ModuleHandle oldHandle = std::move(it->second);
        modules.erase(it);
        
        if (oldHandle.module) {
            oldHandle.module->stop();
            oldHandle.info.isRunning = false;
            cleanupModuleResources(oldHandle);
        }
        
        // Step 2: New module load karo
        std::cout << "Step 2: Loading new module..." << std::endl;
        bool loadSuccess = false;
        
        // Temporary mutex unlock for loading
        moduleMutex.unlock();
        loadSuccess = loadModule(libraryPath);
        moduleMutex.lock();
        
        if (!loadSuccess) {
            std::cerr << "HOT-SWAP FAILED: Failed to load new module" << std::endl;
            return false;
        }
        
        std::cout << "HOT-SWAP SUCCESS: " << moduleName << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "HOT-SWAP EXCEPTION: " << e.what() << std::endl;
        return false;
    }
}

// Print all modules status
void ModuleManager::printAllModules() const {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    std::cout << "\n === LOADED MODULES ===" << std::endl;
    std::cout << "Total Modules: " << modules.size() << std::endl;
    
    if (modules.empty()) {
        std::cout << "No modules loaded." << std::endl;
        return;
    }
    
    for (const auto& pair : modules) {
        const auto& info = pair.second.info;
        auto uptime = info.getUptime();
        
        std::cout << "├─ " << info.name << " v" << info.version << std::endl;
        std::cout << "│  Status: " << (info.isRunning ? "RUNNING" : "STOPPED") << std::endl;
        std::cout << "│  Health: " << (info.isHealthy ? "HEALTHY" : "UNHEALTHY") << std::endl;
        std::cout << "│  Uptime: " << uptime.count() << " seconds" << std::endl;
        std::cout << "│  Path: " << info.libraryPath << std::endl;
    }
    std::cout << "=========================" << std::endl;
}


void ModuleManager::shutdown() {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    std::cout << "\nSYSTEM SHUTDOWN STARTED" << std::endl;
    std::cout << "Unloading " << modules.size() << " modules..." << std::endl;
    
    for (auto& pair : modules) {
        ModuleHandle& handle = pair.second;
        
        if (handle.module) {
            std::cout << "Stopping: " << handle.info.name << std::endl;
            handle.module->stop();
            handle.info.isRunning = false;
        }
    }
    
    auto it = modules.begin();
    while (it != modules.end()) {
        ModuleHandle& handle = it->second;
        cleanupModuleResources(handle);
        it = modules.erase(it); 
    }
    
    std::cout << "SYSTEM SHUTDOWN COMPLETED" << std::endl;
}