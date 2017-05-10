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
	int last;
    int current;
    int next;
    //a state
    struct State
    {
        Function< void (void) > start;
        Function< void (float)> run;
        Function< void (void) > end;
    };
    std::map< int , State > states;
    //event: run
    virtual void onRun(float dt);

    public:

		StateManager() :last(-1)
					   ,current(-1)
					   ,next(-1)
    {
    }
        
    StateManager* addState(int name, Function<void (float)> srun );
    StateManager* addState(int name, Function<void (void) > sstart,
                                     Function<void (float)> srun,
                                     Function<void (void) > send=nullptr );
    void eraseState(int name);

    void setNextState(int stateid);
    void setCurrentState(int stateid);

	int getCurrentState();
	int getNextState();
	int getLastState();
    //component
    ADD_COMPONENT_METHOS(StateManager)
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);

};
REGISTERED_COMPONENT(StateManager,"StateManager")

};

#endif
