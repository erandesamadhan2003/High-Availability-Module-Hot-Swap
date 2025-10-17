#include "ModuleManager.hpp"
#include "../utils/Logger.hpp"
#include "HealthMonitor.hpp"
#include <iostream>
#include <dlfcn.h>
#include <fstream>
#include <sstream>
#include <set>

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
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto& logger = Logger::getInstance();
    auto& healthMonitor = HealthMonitor::getInstance();
    
    logger.info("Loading module: " + libraryPath, "ModuleManager");
    auto loadStartTime = std::chrono::steady_clock::now();

    try {
        // Step 1: Library load karo
        auto library = std::make_unique<DynamicLibrary>(libraryPath);
        if (!library->isLoaded()) {
            logger.error("Failed to load library: " + libraryPath, "ModuleManager");
            return false;
        }

        logger.debug("Library loaded successfully: " + libraryPath, "ModuleManager");

        // Step 2: Factory functions get karo
        using CreateFunc = IModule* (*)();
        auto createModule = (CreateFunc)library->getFunction("createModule");
        auto destroyModule = (void (*)(IModule*))library->getFunction("destroyModule");

        if (!createModule || !destroyModule) {
            logger.error("Factory functions not found in: " + libraryPath, "ModuleManager");
            return false;
        }

        logger.debug("Factory functions found", "ModuleManager");

        // Step 3: Module create karo
        IModule* module = createModule();
        if (!module) {
            logger.error("Failed to create module from: " + libraryPath, "ModuleManager");
            return false;
        }

        // Step 4: Module initialize karo
        if (!module->init()) {
            logger.error("Module initialization failed: " + libraryPath, "ModuleManager");
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

        // Step 7: Map mein store karo
        modules[info.name] = std::move(handle);

        // Step 8: Module start karo
        module->start();
        modules[info.name].info.isRunning = true;
        modules[info.name].info.isHealthy = true;

        // Register with health monitor - FIXED LAMBDA
        auto healthCheckFunction = [this, moduleName = info.name]() -> bool {
            auto* modulePtr = this->getModule(moduleName);
            return modulePtr ? modulePtr->isHealthy() : false;
        };
        healthMonitor.registerModule(info.name, healthCheckFunction);

        // Record metrics
        auto loadEndTime = std::chrono::steady_clock::now();
        auto loadTime = std::chrono::duration_cast<std::chrono::milliseconds>(loadEndTime - loadStartTime);
        healthMonitor.recordModuleLoad(info.name, loadTime);

        logger.info("Module loaded successfully: " + info.name + " v" + info.version + 
                   " (load time: " + std::to_string(loadTime.count()) + "ms)", "ModuleManager");
        return true;

    } catch (const std::exception& e) {
        logger.error("Exception in loadModule: " + std::string(e.what()), "ModuleManager");
        return false;
    }
}

bool ModuleManager::unloadModule(const std::string& moduleName) {
    std::lock_guard<std::mutex> lock(moduleMutex);
    
    auto& logger = Logger::getInstance();
    auto& healthMonitor = HealthMonitor::getInstance();
    
    logger.info("Unloading module: " + moduleName, "ModuleManager");

    auto it = modules.find(moduleName);
    if (it == modules.end()) {
        logger.warning("Module not found for unloading: " + moduleName, "ModuleManager");
        return false;
    }

    try {
        ModuleHandle& handle = it->second;
        
        // Step 1: Module stop karo
        if (handle.module) {
            handle.module->stop();
            handle.info.isRunning = false;
            logger.debug("Module stopped: " + moduleName, "ModuleManager");
        }

        // Unregister from health monitor
        healthMonitor.unregisterModule(moduleName);
        
        // Record metrics
        healthMonitor.recordModuleUnload(moduleName);

        // Step 2: Cleanup karo
        cleanupModuleResources(handle);

        // Step 3: Map se remove karo
        modules.erase(it);
        
        logger.info("Module unloaded successfully: " + moduleName, "ModuleManager");
        return true;

    } catch (const std::exception& e) {
        logger.error("Exception in unloadModule: " + std::string(e.what()), "ModuleManager");
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
    
    auto& logger = Logger::getInstance();
    auto& healthMonitor = HealthMonitor::getInstance();
    
    logger.info("Hot-swap started for module: " + moduleName, "ModuleManager");

    auto it = modules.find(moduleName);
    if (it == modules.end()) {
        logger.error("Module not found for hot-swap: " + moduleName, "ModuleManager");
        return false;
    }

    std::string libraryPath = it->second.info.libraryPath;
    
    try {
        // Step 1: Old module unload karo
        logger.debug("Unloading old module: " + moduleName, "ModuleManager");
        ModuleHandle oldHandle = std::move(it->second);
        modules.erase(it);
        
        if (oldHandle.module) {
            oldHandle.module->stop();
            oldHandle.info.isRunning = false;
            cleanupModuleResources(oldHandle);
        }
        
        // Step 2: New module load karo
        logger.debug("Loading new module: " + libraryPath, "ModuleManager");
        bool loadSuccess = false;
        
        // Temporary mutex unlock for loading
        moduleMutex.unlock();
        loadSuccess = loadModule(libraryPath);
        moduleMutex.lock();
        
        if (!loadSuccess) {
            logger.error("Hot-swap failed: Failed to load new module", "ModuleManager");
            healthMonitor.recordHotSwap(moduleName, false);
            return false;
        }
        
        healthMonitor.recordHotSwap(moduleName, true);
        logger.info("Hot-swap successful: " + moduleName, "ModuleManager");
        return true;
        
    } catch (const std::exception& e) {
        healthMonitor.recordHotSwap(moduleName, false);
        logger.error("Hot-swap exception: " + std::string(e.what()), "ModuleManager");
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
    
    auto& logger = Logger::getInstance();
    logger.info("System shutdown started. Unloading " + std::to_string(modules.size()) + " modules", "ModuleManager");
    
    for (auto& pair : modules) {
        ModuleHandle& handle = pair.second;
        
        if (handle.module) {
            logger.debug("Stopping module: " + handle.info.name, "ModuleManager");
            handle.module->stop();
            handle.info.isRunning = false;
            cleanupModuleResources(handle);
        }
    }
    
    modules.clear();
    logger.info("System shutdown completed", "ModuleManager");
}

void ModuleManager::scanAndLogRuntimeSharedLibraries() const {
    auto& logger = Logger::getInstance();

    std::set<std::string> runtimeLibs;
    std::ifstream maps("/proc/self/maps");
    if (!maps.is_open()) {
        logger.warning("Unable to open /proc/self/maps to scan runtime libraries", "ModuleManager");
        return;
    }

    std::string line;
    while (std::getline(maps, line)) {
        // Last token (if present) is the mapped path
        std::istringstream iss(line);
        std::string addr, perms, offset, dev, inode, path;
        if (!(iss >> addr >> perms >> offset >> dev >> inode)) {
            continue;
        }
        if (!(iss >> path)) {
            continue;
        }
        // Only keep real file-backed shared objects
        if (path.size() > 3 && path.find(".so") != std::string::npos) {
            runtimeLibs.insert(path);
        }
    }
    maps.close();

    // Snapshot managed module library paths
    std::set<std::string> managedPaths;
    {
        std::lock_guard<std::mutex> lock(moduleMutex);
        for (const auto& pair : modules) {
            managedPaths.insert(pair.second.info.libraryPath);
        }
    }

    logger.info("Runtime shared library scan: found " + std::to_string(runtimeLibs.size()) + " .so mappings", "ModuleManager");

    for (const auto& lib : runtimeLibs) {
        bool managed = managedPaths.find(lib) != managedPaths.end();
        logger.info(std::string(managed ? "[MANAGED] " : "[UNMANAGED] ") + lib, "ModuleManager");
    }

    // Optionally: log managed modules that are not present in runtime libs
    for (const auto& mp : managedPaths) {
        if (runtimeLibs.find(mp) == runtimeLibs.end()) {
            logger.warning("Managed module library not present in runtime maps: " + mp, "ModuleManager");
        }
    }
}