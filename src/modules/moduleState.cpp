#include "modules/moduleState.h"

namespace ModuleState {
    const char* moduleStateToString(Modulestate state) {
        switch (state) {
            case Modulestate::UNLOADED: return "UNLOADED";
            case Modulestate::ACTIVE: return "ACTIVE";
            case Modulestate::LOADING: return "LOADING";
            case Modulestate::STANDBY: return "STANDBY";
            case Modulestate::FAILED: return "FAILED";
            default: return "UNKNOWN";
        }
    }

    
}