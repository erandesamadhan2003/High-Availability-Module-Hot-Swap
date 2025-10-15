#include <iostream>
#include <dlfcn.h> // Linux dynamic loading function
#include "DynamicLibrary.hpp"

// Constructor - library load 
DynamicLibrary::DynamicLibrary(const std::string& libraryPath) : handle(nullptr), path(libraryPath) {
    
    std::cout << "Loading library: " << libraryPath << std::endl;
    
    // Library load using dlopen
    handle = dlopen(libraryPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    
    if (!handle) {
        std::cerr << "ERROR loading library: " << dlerror() << std::endl;
    } else {
        std::cout << "Library loaded successfully: " << libraryPath << std::endl;
    }
}

// Destructor - library unload
DynamicLibrary::~DynamicLibrary() {
    if (handle) {
        std::cout << "Unloading library: " << path << std::endl;
        dlclose(handle);  // Library close 
        handle = nullptr;
        std::cout << "Library unloaded: " << path << std::endl;
    }
}

void* DynamicLibrary::getFunction(const std::string& functionName) {
    // check library loaded or not
    if (!handle) {
        std::cerr << "Library not loaded, cannot get function: " << functionName << std::endl;
        return nullptr;
    }

    // Function pointer using dlsym
    void* function = dlsym(handle, functionName.c_str());
    
    if (!function) {
        std::cerr << "Function not found: " << functionName << " - " << dlerror() << std::endl;
    } else {
        std::cout << "Function found: " << functionName << std::endl;
    }
    
    return function;
}

bool DynamicLibrary::isLoaded() const {
    return handle != nullptr;
}