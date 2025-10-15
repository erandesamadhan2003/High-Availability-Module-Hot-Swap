#pragma once
#include <string>

class DynamicLibrary {
private:
    void* handle = nullptr;
    std::string path;

public:
    DynamicLibrary(const std::string& libraryPath);
    ~DynamicLibrary();
    
    void* getFunction(const std::string& functionName);
    bool isLoaded() const;
    std::string getPath() const { return path; }

    // Copying prevent karne ke liye
    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;
};