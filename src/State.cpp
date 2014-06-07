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

int StateInterface::getLastMessage()
{
    return _manager->getLastMessage();
}
void StateInterface::sendMessage(int msg)
{
    _manager->sendMessage(msg);
}
////////////////////////////////////////

bool StateManager::exists(int state)
{
    return states.find(state)!=states.end();
}
void StateManager::_setNextState(float dt)
{
    if(getCuttentState())
    {
        //can translate?
        if(getCuttentState()->onStateTranslate(dt))
        {
            //call event
            getCuttentState()->onStateEnd();
            //change state
            current=nextState;
            nextState=-1;
            //call event
            getCuttentState()->onStateStart();
        }
    }
    else
    {
        //change state
        current=nextState;
        nextState=-1;
        //call event
        getCuttentState()->onStateStart();

    }
}

//parents methos
void StateManager::addChild(StateManager *child,bool destructible)
{
    DEBUG_ASSERT(child->smParent==NULL);
    child->smParent=this;
    child->destructible=destructible;
    smChilds.push_back(child);
}
void StateManager::erseChild(StateManager *child)
{
    if(child->smParent==this)
    {
        smChilds.remove(child);
        child->smParent=NULL;
    }
}
//application methos
void StateManager::update(float dt)
{
    if(getNextState())
        _setNextState(dt);
    if(getCuttentState())
        getCuttentState()->onStateRun(dt);
    //update childs
    for(auto child:smChilds)
        child->update(dt);
}

//init
StateManager::StateManager()
    :message(-1)
    ,current(-1)
    ,nextState(-1)
    ,smParent(NULL)
    ,destructible(false) {}

StateManager::~StateManager()
{
    //destoy states
    for(auto state:states)
        if(state.second.destructible)
            delete state.second.ptr;
    states.clear();
    //destoy childs
    for(auto child:smChilds)
        if(child->destructible)
            delete child;
    smChilds.clear();
}

//state manager
void StateManager::addState(int stateid,StateInterface* state,bool destructible)
{
    DEBUG_ASSERT(!exists(stateid));
    states[stateid]=state ? State(state,destructible) : State(this,false);
    states[stateid].ptr->setManager(this);
}
void StateManager::setNextState(int stateid)
{
    nextState=stateid;
}
void StateManager::setCurrentState(int stateid)
{
    setNextState(stateid);
    _setNextState(Application::instance()->getLastDeltaTime());
}
//add event
void StateManager::sendMessage(int msg)
{
    message=msg;
    if(getCuttentState())
        getCuttentState()->onStateMessage(msg);
}


//query
int StateManager::getLastMessage()
{
    return message;
}
int StateManager::getCurrentStateID()
{
    return current;
}
int StateManager::getNextStateID()
{
    return nextState;
}
StateInterface *StateManager::getCuttentState()
{
    if(current!=-1)
        if(exists(current))
            return states[current].ptr;
    return NULL;
}
StateInterface *StateManager::getNextState()
{
    if(nextState!=-1)
        if(exists(nextState))
            return states[nextState].ptr;
    return NULL;
}

//utility methos
Easy2D::Screen* StateManager::getScreen()
{
    return Application::instance()->getScreen();
}
Easy2D::Audio* StateManager::getAudio()
{
    return Application::instance()->getAudio();
}
Easy2D::Input* StateManager::getInput()
{
    return Application::instance()->getInput();
}
Easy2D::Game* StateManager::getGame()
{
    return Application::instance()->getGame();
}
Easy2D::ResourcesGroup* StateManager::getResourcesGroup(const String& name)
{
    return Application::instance()->getResourcesGroup(name);
}