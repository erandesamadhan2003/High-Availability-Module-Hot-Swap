#ifndef MODULE_STATE_H
#define MODULE_STATE_H
#include <string>
namespace Modules {
    enum class ModuleState {
        UNLOADED,
        LOADING,
        ACTIVE,
        STANDBY, 
        FAILED
    };

    const char* moduleStateToString(ModuleState state);

    ModuleState stringToModuleState(ModuleState state);
}

#endif

