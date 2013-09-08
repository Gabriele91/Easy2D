#ifndef STATEINTERFACE_H
#define STATEINTERFACE_H

#include <Config.h>
#include <Debug.h>

//FSM event-driven: http://en.wikipedia.org/wiki/Event-driven_finite-state_machine

namespace Easy2D {

    
    /////////////////////
    class StateManager;
    class StateInterface;
    /////////////////////
    
    class StateInterface{
    
        StateManager* _manager;
        friend class StateManager;
        void setManager(StateManager* manager){
            _manager=manager;
        }
        
    public:
        
        //manager
        StateManager* getManager(){ return _manager; }
        int getLastMessage();
        void sendMessage(int msg);
        
        //update state
        virtual bool onStateTranslate(float dt){ return true; }
        virtual void onStateMessage(int message){}
        
        //state events
        virtual void onStateStart(){}
        virtual void onStateRun(float dt){}
        virtual void onStateEnd(){}
    };
    
    class StateLambda : public StateInterface{
        
        std::function<bool (float)> _onStateTranslate;
        std::function<void ()> _onStateStart;
        std::function<void (float)> _onStateRun;
        std::function<void (int)> _onStateMessage;
        std::function<void ()> _onStateEnd;
        
    public:
        
        StateLambda(const std::function<void (float)>& _onStateRun=[](float){}):
        _onStateTranslate([](float)->bool{return true;}),
        _onStateStart([](){}),
        _onStateRun(_onStateRun),
        _onStateMessage([](int){}),
        _onStateEnd([](){})
        {}
        
        StateLambda(const std::function<void (int)>& _onStateMessage,
                    const std::function<void (float)>& _onStateRun):
        _onStateTranslate([](float)->bool{return true;}),
        _onStateStart([](){}),
        _onStateRun(_onStateRun),
        _onStateMessage(_onStateMessage),
        _onStateEnd([](){})
        {}
        
        StateLambda(
           const std::function<void ()>& _onStateStart,
           const std::function<void (float)>& _onStateRun,
           const std::function<void ()>& _onStateEnd,
           const std::function<void (int)>& _onStateMessage=[](int){},
           const std::function<bool (float)>& _onStateTranslate=[](float)->bool{return true;}
        )
        :_onStateTranslate(_onStateTranslate),
         _onStateStart(_onStateStart),
         _onStateRun(_onStateRun),
         _onStateMessage(_onStateMessage),
         _onStateEnd(_onStateEnd)
        {}
        
        //update state
        virtual bool onStateTranslate(float dt){ return _onStateTranslate(dt); }
        virtual void onStateMessage(int message){ return _onStateMessage(message); }
        
        //state events
        virtual void onStateStart(){ _onStateStart(); }
        virtual void onStateRun(float dt){ _onStateRun(dt); }
        virtual void onStateEnd(){ _onStateEnd(); }
        
    };
    
	class StateManager : public StateInterface {
        
        struct State{
            
            StateInterface* ptr;
            bool destructible;
            
            State()
            :ptr(NULL),destructible(false){}
            
            State(StateInterface* ptr,bool destructible=true)
            :ptr(ptr),destructible(destructible){}
            
            bool exist(){
                return ptr!=NULL;
            }
            
        };
        
        //////////////////////////////////////
        int message;
        int current;
        int nextState;
        DUNORDERED_MAP < int , State > states;
        //////////////////////////////////////
        bool exists(int state);
        void _setNextState(float dt);
        //////////////////////////////////////
        std::list< StateManager * > smChilds;
        StateManager *smParent;
        bool destructible;
        //////////////////////////////////////
        
    public:
        
        //application methos
        virtual void update(float dt);
        //parents methos
        void addChild(StateManager *child,bool destructible=true);
		void erseChild(StateManager *child);
        
        //init
        StateManager();
        virtual ~StateManager();
        
        //state manager
        void addState(int stateid,StateInterface* state=NULL,bool destructible=true);
        void setNextState(int stateid);
        void setCurrentState(int stateid);
        //add event
        void sendMessage(int msg);
        
        //query
        int getLastMessage();
        int getCurrentStateID();
        int getNextStateID();
        StateInterface *getCuttentState();
        StateInterface *getNextState();

	};

};

#endif