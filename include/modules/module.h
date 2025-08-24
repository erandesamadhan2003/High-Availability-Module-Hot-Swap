#ifndef MODULE_H
#define MODULE_H
#include "./moduleState.h"
#include <string>

namespace Module {
    class Module_H{
    protected: 
        ModuleState::Modulestate currentState;
        std::string modulename;
        int moduleId;

    public: 
        Module_H(const std::string& name, int id);

        virtual ~Module_H();

        virtual bool unLoad() = 0;
        virtual bool activate() = 0;
        virtual bool deactivate() = 0;
        ModuleState::Modulestate getName() const;

        int getId () const;

        virtual void printStatus() = 0;
    };
}

#endif