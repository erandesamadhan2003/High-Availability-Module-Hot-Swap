#include <iostream>
#include "ModuleManager.hpp"

ModuleManager* ModuleManager::instance = nullptr;

ModuleManager& ModuleManager::getInstance() {
    if (!instance) {
        instance = new ModuleManager();
    }
    return *instance;
}

bool ModuleManager::loadModule(const std::string& libraryPath) {
    std::cout << "Loading module from: " << libraryPath << std::endl;
    return true; 
}

void ModuleManager::printAllModules() {
    std::cout << "Module Manager is working!" << std::endl;
}