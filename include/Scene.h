#ifndef SCENE_H
#define SCENE_H

#include <Config.h>
#include <State.h>
#include <EStack.h>
#include <Debug.h>
#include <Render.h>
#include <World.h>

namespace Easy2D {

    class Screen;
	class Audio;
	class Input;
    class Game;
    
	class Scene : public Render, //Graphics
				  public World,  //Physics
                  public StateManager {
        
        struct SubScene{
            Scene *child;
            bool destructible;
            
            SubScene():child(NULL),destructible(false){}
            SubScene(Scene *child,bool destructible=true):child(child),destructible(destructible){}
        };
                      
        DUNORDERED_MAP<int,SubScene> scenes;
        EStack<int> actives;
                      
        bool isStarted;
        void _onStartResume(){
            if(isStarted) onResume();
            else{ onStart(); isStarted=true; }
        }
        void _onRunLogic(float dt){
            //update state machine
            StateManager::update(dt);
            //update logic scene
            onRun(dt);
            //update logic render
            Render::update(dt);
        }
        void _onRunDraw(){
            //draw scene
           // Render::draw();
        }
          

	public:
        
        bool isContent(int uid){
            return scenes.find(uid)!=scenes.end();
        }
        bool isActive(int uid){
            return actives.contains(uid);
        }
        Scene* getScene(int uid){
            if(isContent(uid))
                return scenes[uid].child;
            return NULL;
        }
        int getCurrentUID(){
          return actives.top();
        }
        Scene* getCurrentScene(){
            if(actives.empty()) return NULL;
            return scenes[actives.top()].child;
        }
                      
        //add sub scene
        void addScene(int uid,Scene* scene,bool destructible=true){
            scenes[uid]=SubScene(scene,destructible);
        }
        void addSceneAndActive(int uid,Scene* scene,bool destructible=true){
            addScene(uid,scene,destructible);
            active(uid);
        }
        //active a scene
        void active(int uid){
            DEBUG_ASSERT(isContent(uid));
            //pause last scene
            if(actives.size())
                scenes[actives.top()].child->onPause();
            //start last scene
            scenes[uid].child->_onStartResume();
            actives.push(uid);
            
        }
        //pop a scene
        void pop(){
            //pause last scene
            if(actives.size()){
                scenes[actives.top()].child->onPause();
                actives.pop();
            }
        }
        //erase a scene
        Scene *erase(int uid){
          DEBUG_ASSERT(isContent(uid));
          //get child
          auto it=scenes.find(uid);
          Scene *temp=it->second.child;
          //if olready active
          while(actives.size() && actives.top()==uid)
              pop();
          //disable activation
          while(actives.contains(uid))
              actives.erase(uid);
          //delete from map
          scenes.erase(it);
          //return
          return temp;
        }
        
        //scene methos
        virtual void onStart()=0;
        virtual void onResume(){}
        virtual void onRun(float dt)=0;
        virtual void onPause(){}
        virtual void onEnd()=0;
                      
        //costructor
        Scene():isStarted(false){}
                      
        //destoy a scene
        virtual ~Scene(){
            //destoy all sub scene
            for(auto scene:scenes)
                if(scene.second.destructible)
                    delete scene.second.child;
        }
				
        //application methos
        virtual void start(){
            _onStartResume();
        }
        virtual void run(float dt){
            //update logic 
            _onRunLogic(dt);
            //update logic child
            if(actives.size())
                scenes[actives.top()].child->_onRunLogic(dt);
            //draw all
            _onRunDraw();
            //draw child
            if(actives.size())
                scenes[actives.top()].child->_onRunDraw();
        }
        virtual void end(){
            //call onEnd for all child activated
            for(auto scene:scenes)
                if(scene.second.child->isStarted)
                    scene.second.child->onEnd();
            //end scene
            onEnd();
        }    

	};

};

#endif