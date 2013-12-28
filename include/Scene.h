#ifndef SCENE_H
#define SCENE_H

#include <Config.h>
#include <State.h>
#include <EStack.h>
#include <Debug.h>
#include <Render.h>
#include <ResourcesGroup.h>

namespace Easy2D {

    class Screen;
	class Audio;
	class Input;
    class Game;
    
	class Scene : public Render,
                  public ResourcesGroup,
                  public StateManager {
        
        struct SubScene{
            Scene *child;
            bool destructible;
            
            SubScene():child(NULL),destructible(false){}
            SubScene(Scene *child,bool destructible=true):child(child),destructible(destructible){}
        };
                      
        DUNORDERED_MAP<int,SubScene> scenes;
        EStack<int> active;
                      
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
        
        bool sceneExist(int uid){
            return scenes.find(uid)!=scenes.end();
        }
        bool sceneActive(int uid){
            return active.contains(uid);
        }
        Scene* getScene(int uid){
            if(sceneExist(uid))
                return scenes[uid].child;
            return NULL;
        }
                      
        int getCurrentUID(){
          return active.top();
        }
                      
        Scene* getCurrentScene(){
            if(active.empty()) return NULL;
            return scenes[active.top()].child;
        }
                      
        //add sub scene
        void addScene(int uid,Scene* scene,bool destructible=true){
            scenes[uid]=SubScene(scene,destructible);
        }
        void addSceneAndActive(int uid,Scene* scene,bool destructible=true){
            addScene(uid,scene,destructible);
            activeScene(uid);
        }
        //active a scene
        void activeScene(int uid){
            DEBUG_ASSERT(sceneExist(uid));
            //pause last scene
            if(active.size())
                scenes[active.top()].child->onPause();
            //start last scene
            scenes[uid].child->_onStartResume();
            active.push(uid);
            
        }
        void popScene(){
            //pause last scene
            if(active.size()){
                scenes[active.top()].child->onPause();
                active.pop();
            }
        }
        
                      
        Scene *eraseScene(int uid){
          DEBUG_ASSERT(sceneExist(uid));
          //get child
          auto it=scenes.find(uid);
          Scene *temp=it->second.child;
          //if olready active
          while(active.size() && active.top()==uid)
              popScene();
          //disable activation
          while(active.contains(uid))
              active.erase(uid);
          //delete from map
          scenes.erase(it);
          //return
          return temp;
        }
        
        //application methos
        virtual void start(){
            _onStartResume();
        }
        virtual void run(float dt){
            //update logic 
            _onRunLogic(dt);
            //update logic child
            if(active.size())
                scenes[active.top()].child->_onRunLogic(dt);
            //draw all
            _onRunDraw();
            //draw child
            if(active.size())
                scenes[active.top()].child->_onRunDraw();
        }
        virtual void end(){
            //call onEnd for all child activated
            for(auto scene:scenes)
                if(scene.second.child->isStarted)
                    scene.second.child->onEnd();
            //end scene
            onEnd();
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
        //utility methos
        Screen* getScreen();
        Audio* getAudio();
        Input* getInput();
        Game* getGame();

	};

};

#endif