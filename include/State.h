#ifndef STATEINTERFACE_H
#define STATEINTERFACE_H

#include <Config.h>
#include <Application.h>
#include <Debug.h>
#include <Component.h>

//FSM event-driven: http://en.wikipedia.org/wiki/Event-driven_finite-state_machine

namespace Easy2D
{

    class StateManager : public Component
    {
        //friend whit object
        friend class Object;
        //states
        int current;
        int next;
        //a state
        struct State
        {
            DFUNCTION< void (void) > start;
            DFUNCTION< void (float)> run;
            DFUNCTION< void (void) > end;
        };
        std::map< int , State > states;
        //event: run
        virtual void onRun(float dt);

    public:

        StateManager():current(-1)
                      ,next(-1)
        {
        }
        
        StateManager* addState(int name, DFUNCTION<void (float)> srun );
        StateManager* addState(int name, DFUNCTION<void (void) > sstart,
                                         DFUNCTION<void (float)> srun,
                                         DFUNCTION<void (void) > send=nullptr );
        void eraseState(int name);

        void setNextState(int stateid);
        void setCurrentState(int stateid);

        int getCurrentState();
        int getNextState();
        //component
        virtual const char* getComponentName() const
        {
            return "StateManager";
        }
        static const  type_info* getComponentType()
        {
            return &typeid(StateManager);
        }

    };

};

#endif