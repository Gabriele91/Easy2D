//
//  State.cpp
//  Easy2D
//
//  Created by Gabriele Di Bari on 09/08/13.
//
//
#include <stdafx.h>
#include <State.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////
//event: run
void StateManager::onRun(float dt)
{
    if(current>=0)
    {
        states[current].run(dt);
    }
    //set next state
    if(next>=0)
    {
        //end state
        if(states[current].end) 
            states[current].end();
        //next state
		last   = current;
        current= next;
        //start state
        if(states[current].start) 
            states[current].start();        
        //void next
        next=-1;
    }
};

StateManager* StateManager::addState(int name, Function<void (float)> srun)
{
    return addState(name,nullptr,srun,nullptr);
}
StateManager* StateManager::addState(int name, Function<void (void) > sstart,
                                               Function<void (float)> srun,
                                               Function<void (void) > send )
{
    State state;
    state.start=sstart;
    state.run=srun;
    state.end=send;
    states[name]=state;
    return this;
}
void StateManager::eraseState(int name)
{
    states.erase(name);
}

void StateManager::setNextState(int stateid)
{
	next = stateid;
}
void StateManager::setCurrentState(int stateid)
{
    //call end
    if(current>=0 && states[current].end) 
        states[current].end();
    //set
	last   = current;
    current= stateid;
    //call start
    if(current>=0 && states[current].start) 
        states[current].start();
}

int StateManager::getCurrentState()
{
    return current;
}
int StateManager::getNextState()
{
	return next;
}
int StateManager::getLastState()
{
	return last;
}


void StateManager::serialize(Table& table)
{
	Table& rstate = table;
	if (last >= 0) rstate.set("last", (float)current);
	if (current >= 0) rstate.set("current", (float)current);
    if (next>=0)    rstate.set("next",(float)next);
}
void StateManager::deserialize(const Table& table)
{
	last = (int)table.getFloat("last", -1);
	current = (int)table.getFloat("current", -1);
    next   =(int)table.getFloat("next",-1);
}
