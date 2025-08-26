#include <iostream> 
#include "modules/module.h"
#include "modules/moduleState.h"
namespace Modules {
    Module::Module(const std::string& name, int id) : moduleName(name), moduleId(id), currentState(ModuleState::UNLOADED) {}

    Module::~Module() {
        std::cout << "Module '" << moduleName << "' destroyed" << std::endl;
    }

    ModuleState Module::getState() const {
        return currentState;
    }

    std::string Module::getName() const {
        return moduleName;
    }

    int Module::getId() const {
        return moduleId;
    }

    void Module::setState(ModuleState state) {
        currentState = state;
    }
}