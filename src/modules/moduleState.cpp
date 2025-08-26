#include "modules/moduleState.h"

namespace Modules {
    const char* moduleStateToString(ModuleState state) {
        switch (state) {
            case ModuleState::UNLOADED: return "UNLOADED";
            case ModuleState::ACTIVE: return "ACTIVE";
            case ModuleState::LOADING: return "LOADING";
            case ModuleState::STANDBY: return "STANDBY";
            case ModuleState::FAILED: return "FAILED";
            default: return "UNKNOWN";
        }
    }

    ModuleState stringToModuleState(ModuleState state) {
    }
}
