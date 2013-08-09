#ifndef STATEINTERFACE_H
#define STATEINTERFACE_H

#include <Config.h>

namespace Easy2D {

    class StateInterface{
    
    public:
        //update state
        virtual bool onStateTranslate(float dt){ return true; }
        
        //state events
        virtual void onStateStart(int uid){}
        virtual void onStateRun(int uid,float dt){}
        virtual void onStateEnd(int uid){}
    };
    
    class StateLambda : public StateInterface{
        
        std::function<bool (float)> _onStateTranslate;
        std::function<void (int)> _onStateStart;
        std::function<void (int,float)> _onStateRun;
        std::function<void (int)> _onStateEnd;
        
    public:
        
        StateLambda(
            std::function<bool (float)> _onStateTranslate,
            std::function<void (int)> _onStateStart,
            std::function<void (int,float)> _onStateRun,
            std::function<void (int)> _onStateEnd
        )
        :_onStateTranslate(_onStateTranslate),
         _onStateStart(_onStateStart),
         _onStateRun(_onStateRun),
         _onStateEnd(_onStateEnd)
        {}
        
        //update state
        virtual bool onStateTranslate(float dt){ return _onStateTranslate(dt); }
        
        //state events
        virtual void onStateStart(int uid){ _onStateStart(uid); }
        virtual void onStateRun(int uid,float dt){ _onStateRun(uid,dt); }
        virtual void onStateEnd(int uid){ _onStateEnd(uid); }
        
    };
    
	class StateManager : public StateInterface {
        
        struct State{
            int uid;
            StateInterface* ptr;
            State():uid(-1),ptr(NULL){}
            bool exist(){
                return (ptr!=NULL || uid!=-1);
            }
        };
        
        State current;
        State nextState;
        
        void _setNextState(float dt){
            //can translate?
            if(current.ptr->onStateTranslate(dt)){
                //call event
                current.ptr->onStateEnd(current.uid);
                //change state
                current=nextState;
                nextState=State();
                //call event
                current.ptr->onStateStart(current.uid);
            }
        }
    
    protected:
        
        //application methos
        virtual void update(float dt){
            if(nextState.exist())
                _setNextState(dt);
            if(current.exist())
                current.ptr->onStateRun(current.uid,dt);
        }
        
        
	public:
        
        //state manager
        void addState(int uid,StateInterface* state=NULL){
            nextState.uid=uid;
            nextState.ptr=state ? state : this;
        }
        virtual ~StateManager(){}

	};

};

#endif