#ifndef MODULE_H
#define MODULE_H
#include "./moduleState.h"
#include <string>

namespace Modules {
    class Module{
    protected: 
        Modules::ModuleState currentState;
        std::string moduleName;
        int moduleId;

    public: 
        Module(const std::string& name, int id);

        virtual ~Module();

        virtual bool load() = 0;
        virtual bool unLoad() = 0;
        virtual bool activate() = 0;
        virtual bool deactivate() = 0;
        ModuleState getState() const;
        std::string getName() const;
       void setState(ModuleState state);    

        int getId () const;

        virtual void printStatus() = 0;
    };

    
}

#endif