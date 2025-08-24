#ifndef MODULE_STATE_H
#define MODULE_STATE_H

namespace ModuleState {
    enum class Modulestate {
        UNLOADED,
        LOADING,
        ACTIVE,
        STANDBY, 
        FAILED
    };

    const char* moduleStateToString(Modulestate state);
}

#endif

