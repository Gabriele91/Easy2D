#ifndef SCENE_H
#define SCENE_H

#include <Config.h>
#include <State.h>
#include <EStack.h>
#include <Debug.h>
#include <Render.h>
#include <World.h>
#include <Object.h>

namespace Easy2D
{

class Screen;
class Audio;
class Input;
class Game;

class Scene : public Render, //Graphics
              public World   //Physics
{

    struct SubScene
    {
        Scene *child;
        bool destructible;

        SubScene():child(NULL),destructible(false) {}
        SubScene(Scene *child,bool destructible=true):child(child),destructible(destructible) {}
    };

    DUNORDERED_MAP<int,SubScene> scenes;
    EStack<int> actives;
    std::list<Object*> objects;

    bool isStarted;
    void onStartResume()
    {
        if(isStarted) onResume();
        else
        {
            onStart();
            isStarted=true;
        }
    }
    void onRunLogic(float dt)
    {
        //update objects
        for(auto obj:objects)
            obj/*.second*/->onSceneRun(dt);
        //update logic scene
        onRun(dt);
        //update physics
        World::physics(dt);
    }
    void onRunDraw()
    { 
        //rebuild queue
        buildQueue(objects);
        //draw scene
        Render::draw();
        //draw debug
        World::physicsDraw(Render::getCamera());
    }

public:

    bool isContent(int uid)
    {
        return scenes.find(uid)!=scenes.end();
    }
    bool isActive(int uid)
    {
        return actives.contains(uid);
    }
    Scene* getScene(int uid)
    {
        if(isContent(uid))
            return scenes[uid].child;
        return NULL;
    }
    int getCurrentUID()
    {
        return actives.top();
    }
    Scene* getCurrentScene()
    {
        if(actives.empty()) return NULL;
        return scenes[actives.top()].child;
    }

    //add object
    template<class T>
    T* addObject(T *obj)
    {
        objects.push_back((Object*)obj);
        ((Object*)obj)->setScene(this);
        return obj;
    }
    void eraseObject(Object *obj)
    {
        auto it=std::find(objects.begin(),
                          objects.end(),
                          obj);
        if(it!=objects.end())
        {
            obj->eraseScene();
            objects.erase(it);
        }
    }
    Object* getObject(const String& argname)
    {
        //names list
        std::vector<String> names;
        argname.split(".",names);
        //search
        if(names.size()!=0)
        {
            for(auto obj:objects)
            {
                Object* tmp=obj->getPrivateObject(names,0);
                if(tmp) return tmp;
            }
        }

        return nullptr;
        
    }
    void clearObjects(bool hardDelete=true)
    {
        if(hardDelete)
        {
            //delete all objects
            for(auto obj:objects)
                delete obj/*.second*/;
        }
        objects.clear();
    }
    //add sub scene
    void addScene(int uid,Scene* scene,bool destructible=true)
    {
        scenes[uid]=SubScene(scene,destructible);
    }
    void addSceneAndActive(int uid,Scene* scene,bool destructible=true)
    {
        addScene(uid,scene,destructible);
        active(uid);
    }
    //active a scene
    void active(int uid)
    {
        DEBUG_ASSERT(isContent(uid));
        //pause last scene
        if(actives.size())
            scenes[actives.top()].child->onPause();
        //start last scene
        scenes[uid].child->onStartResume();
        actives.push(uid);
    }
    //pop a scene
    void pop()
    {
        //pause last scene
        if(actives.size())
        {
            scenes[actives.top()].child->onPause();
            actives.pop();
        }
    }
    //erase a scene
    Scene *erase(int uid)
    {
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
    //deserialize
    void deserialize(const Table& table)
    {
        //world
        World::deserialize(table);
        //objects
        const Table& objects=table.getConstTable("Objects");
        //load objects
        for(auto it:objects)
        {
            if(it.second->asType(Table::TABLE))
            {
                //get table
                const Table& table=it.second->get<Table>();
                DEBUG_ASSERT(it.first.isString());
                addObject(new Object())->deserialize(table);
            }
        }
    }
    //scene methos
    virtual void onStart()=0;
    virtual void onResume() {}
    virtual void onRun(float dt)=0;
    virtual void onPostDraw(float dt){};
    virtual void onPause() {}
    virtual void onEnd()=0;

    //costructor
    Scene():isStarted(false) {}

    //destoy a scene
    virtual ~Scene()
    {
        //delete all sub scene
        for(auto scene:scenes)
            if(scene.second.destructible)
                delete scene.second.child;
        //delete all objects
        for(auto obj:objects)
            delete obj/*.second*/;
    }

    //application methos
    virtual void start()
    {
        onStartResume();
    }
    virtual void run(float dt)
    {
        ///////////////////////////////////////////////
        //update logic
        onRunLogic(dt);
        //update logic child
        if(actives.size())
            scenes[actives.top()].child->onRunLogic(dt);
        ///////////////////////////////////////////////
        //draw all
        onRunDraw();
        //draw child
        if(actives.size())
            scenes[actives.top()].child->onRunDraw();
        ///////////////////////////////////////////////
        //post draw all
        onPostDraw(dt);
        //post draw all
        if(actives.size())
            scenes[actives.top()].child->onPostDraw(dt);
        ///////////////////////////////////////////////
    }
    virtual void end()
    {
        //call onEnd for all child activated
        for(auto scene:scenes)
            if(scene.second.child->isStarted)
                scene.second.child->onEnd();
        //end scene
        onEnd();
    }

    //utility methos
    Screen* getScreen();
    Audio* getAudio();
    Input* getInput();
    Game* getGame();
    ResourcesGroup* getResourcesGroup(const String& name);

};

};

#endif