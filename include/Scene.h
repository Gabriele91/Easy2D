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
    //childs
    DUNORDERED_MAP<int,SubScene> scenes;
    EStack<int> actives;
    std::list<Object*> objects;
    //start methos
	bool isStarted;
	void onStartResume();
    //update logic method
	void onRunLogic(float dt);
    //draw method
	void onRunDraw();
    //pause
    void onPauseScene();
    //resume
    void onResumeScene();
	//friend class
	friend class Game;
    //create scene
    Scene(const String& name,
          uint width,
          uint height,
          uint freamPerSecond = 60,
          bool fullscreen = false,
          int  typeBuffers = 8 /* COLOR32DEPTH24STENCIL8 */,
          int  defaultNoAA = 0 /* NOAA */);
    
protected:
    
    void onStartResumeAll();
    void onResumeAll();
    void onPauseAll();
    void onRunAllLogic(float dt);
    void onRunAllDraw();
    void onPostAllDraw(float dt);
    void onEndAll();

public:

	bool isContent(int uid);
	bool isActive(int uid);
	Scene* getScene(int uid);
	int getCurrentUID();
	Scene* getCurrentScene();

    //Objects
    template<class T>
    T* addObject(T *obj)
    {
        objects.push_back((Object*)obj);
        ((Object*)obj)->setScene(this);
        return obj;
    }
	void eraseObject(Object *obj);
	Object* getObject(const String& argname) const;
	void clearObjects(bool hardDelete = true);
	const std::list<Object*>& getObjects() const;
    std::vector<Object*> picking(const Vec2& point) const;
    //add sub scene
	void addScene(int uid, Scene* scene, bool destructible = true);
	void addSceneAndActive(int uid, Scene* scene, bool destructible = true);
    //active a scene
	void active(int uid);
    //pop a scene
	void pop();
    //erase a scene
	Scene* erase(int uid);
	//physics and render attributes deserialize
	void metaDataDeserialize(const Table& table);
	//physics and render attributes serialize
	void metaDataSerialize(Table& table);
	//deserialize
	void deserialize(const Table& table);
	//serialize
	void serialize(Table& table);
    //scene methos
    virtual void onStart()=0;
    virtual void onResume() {}
    virtual void onRun(float dt)=0;
    virtual void onPostDraw(float dt){};
    virtual void onPause() {}
    virtual void onEnd()=0;

    //costructor
	Scene(bool initRender=true);

    //destoy a scene
	virtual ~Scene();

    //application methos
    virtual void start();
    virtual void restart();
    virtual void run(float dt);
    virtual void pause();
	virtual void end();

    //utility methos
    Screen* getScreen();
    Audio* getAudio();
    Input* getInput();
    Game* getGame();
    ResourcesGroup* getResourcesGroup(const String& name);

};

};

#endif