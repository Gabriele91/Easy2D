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
        current=next;
        //start state
        if(states[current].start) 
            states[current].start();        
        //void next
        next=-1;
    }
};

StateManager* StateManager::addState(int name, DFUNCTION<void (float)> srun)
{
    return addState(name,nullptr,srun,nullptr);
}
StateManager* StateManager::addState(int name, DFUNCTION<void (void) > sstart,
                                               DFUNCTION<void (float)> srun,
                                               DFUNCTION<void (void) > send )
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
    next=stateid;
}
void StateManager::setCurrentState(int stateid)
{
    //call end
    if(current>=0 && states[current].end) 
        states[current].end();
    //set
    current=stateid;
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


void StateManager::serialize(Table& table)
{
    Table& rstate=table;
    if(current>=0) rstate.set("current",(float)current);
    if(next>=0)    rstate.set("next",(float)next);
}
void StateManager::deserialize(const Table& table)
{
    current=(int)table.getFloat("current",-1);
    next   =(int)table.getFloat("next",-1);
}