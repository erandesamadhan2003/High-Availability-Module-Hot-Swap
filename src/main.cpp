#include <iostream>
#include "modules/module.h"
#include "modules/moduleState.h"

/**
 * @brief Test module jo Module class ko inherit karta hai
 */
class TestModule : public Modules::Module {
public:
    TestModule(const std::string& name, int id) : Module(name, id) {}
    
    bool load() override {
        std::cout << "Loading module: " << getName() << std::endl;
        // Yahan actual loading logic aayegi
        Modules::Module::setState(Modules::ModuleState::LOADING);
        return true;
    }
    
    bool unLoad() override {
        std::cout << "Unloading module: " << getName() << std::endl;
        // Yahan actual unloading logic aayegi
        Modules::Module::setState(Modules::ModuleState::UNLOADED);
        return true;
    }
    
    bool activate() override {
        std::cout << "Activating module: " << getName() << std::endl;
        // Yahan actual activation logic aayegi
        Modules::Module::setState(Modules::ModuleState::ACTIVE);
        return true;
    }
    
    bool deactivate() override {
        std::cout << "Deactivating module: " << getName() << std::endl;
        // Yahan actual deactivation logic aayegi
        Modules::Module::setState(Modules::ModuleState::UNLOADED);
        return true;
    }

    void printStatus() override {
        std::cout << "Module [" << getId() << "] - " << getName() << " | State: " << moduleStateToString(getState()) << std::endl;
    }

};

int main() {
    std::cout << "=== Testing Module Interface ===" << std::endl;
    
    // Test module create karo
    TestModule testModule("NetworkDriver", 1);
    
    // Status print karo
    testModule.printStatus();
    
    // Functions test karo
    testModule.load();
    testModule.activate();
    testModule.printStatus();
    
    testModule.deactivate();
    testModule.unLoad();
    testModule.printStatus();
    
    std::cout << "=== Test Completed ===" << std::endl;
    return 0;
}