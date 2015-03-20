#include <stdafx.h>
#include <Scene.h>
#include <Application.h>

///////////////////////
using namespace Easy2D;
///////////////////////

Scene::Scene(const String& app,
			 unsigned int width,
			 unsigned int height,
		 	 unsigned int bites,
			 unsigned int freamPerSecond,
			 bool fullscreen,
			 int dfAA) 
			 :isStarted(false) 
{
	//init context
	Application::instance()
		->getScreen()
		->createWindow(app.c_str(),
		width,
		height,
		bites,
		freamPerSecond,
		fullscreen,
		(Screen::AntiAliasing)dfAA);
	//init render
	Render::init();
}
Scene::Scene(bool initRender) :isStarted(false)
{
	//init render
	if (initRender)
	{
		Render::init();
	}
}
//destoy a scene
Scene::~Scene()
{
	//delete all sub scene
	for (auto scene : scenes)
		if (scene.second.destructible)
			delete scene.second.child;
	//delete all objects
	for (auto obj : objects)
		delete obj/*.second*/;
}
//application methos
void Scene::start()
{
	onStartResume();
}
void Scene::run(float dt)
{
	///////////////////////////////////////////////
	//update logic
	onRunLogic(dt);
	//update logic child
	if (actives.size())
		scenes[actives.top()].child->onRunLogic(dt);
	///////////////////////////////////////////////
	//draw all
	onRunDraw();
	//draw child
	if (actives.size())
		scenes[actives.top()].child->onRunDraw();
	///////////////////////////////////////////////
	//post draw all
	onPostDraw(dt);
	//post draw all
	if (actives.size())
		scenes[actives.top()].child->onPostDraw(dt);
	///////////////////////////////////////////////
}
void Scene::end()
{
	//call onEnd for all child activated
	for (auto scene : scenes)
		if (scene.second.child->isStarted)
			scene.second.child->onEnd();
	//end scene
	onEnd();
}
//methos
void Scene::onStartResume()
{
	if (isStarted)
	{
		for (Object* obj : objects)
		{
			obj->onSceneResume();
		}
		onResume();
	}
	else
	{
		onStart();
		isStarted = true;
	}
}
void Scene::onAllPause()
{
	//go to pause
	onPause();
	//pause obj
	for (Object* obj : objects)
	{
		obj->onScenePause();
	}
}
void Scene::onRunLogic(float dt)
{
	//update objects
	for (auto obj : objects)
		obj/*.second*/->onSceneRun(dt);
	//update logic scene
	onRun(dt);
	//update physics
	World::physics(dt);
}
void Scene::onRunDraw()
{
	//rebuild queue
	buildQueue(objects);
	//draw scene
	Render::draw();
	//draw debug
	World::physicsDraw(Render::getCamera());
}
//publics:
bool Scene::isContent(int uid)
{
	return scenes.find(uid) != scenes.end();
}
bool Scene::isActive(int uid)
{
	return actives.contains(uid);
}
Scene* Scene::getScene(int uid)
{
	if (isContent(uid))
		return scenes[uid].child;
	return NULL;
}
int Scene::getCurrentUID()
{
	return actives.top();
}
Scene*Scene::getCurrentScene()
{
	if (actives.empty()) return NULL;
	return scenes[actives.top()].child;
}
//Objects
void Scene::eraseObject(Object *obj)
{
	auto it = std::find(objects.begin(),
		objects.end(),
		obj);
	if (it != objects.end())
	{
		obj->eraseScene();
		objects.erase(it);
	}
}
Object* Scene::getObject(const String& argname) const
{
	//names list
	std::vector<String> names;
	argname.split(".", names);
	//search
	if (names.size() != 0)
	{
		for (auto obj : objects)
		{
			Object* tmp = obj->getPrivateObject(names, 0);
			if (tmp) return tmp;
		}
	}

	return nullptr;

}
void Scene::clearObjects(bool hardDelete)
{
	if (hardDelete)
	{
		//delete all objects
		for (auto obj : objects)
			delete obj/*.second*/;
	}
	objects.clear();
}
const std::list<Object*>& Scene::getObjects() const
{
	return objects;
}
//add sub scene
void Scene::addScene(int uid, Scene* scene, bool destructible)
{
	scenes[uid] = SubScene(scene, destructible);
}
void Scene::addSceneAndActive(int uid, Scene* scene, bool destructible)
{
	addScene(uid, scene, destructible);
	active(uid);
}
//active a scene
void Scene::active(int uid)
{
	DEBUG_ASSERT(isContent(uid));
	//pause last scene
	if (actives.size())
	{
		scenes[actives.top()].child->onAllPause();
	}
	//start last scene
	scenes[uid].child->onStartResume();
	actives.push(uid);
}
//pop a scene
void Scene::pop()
{
	//pause last scene
	if (actives.size())
	{
		scenes[actives.top()].child->onAllPause();
		actives.pop();
	}
}
//erase a scene
Scene* Scene::erase(int uid)
{
	DEBUG_ASSERT(isContent(uid));
	//get child
	auto it = scenes.find(uid);
	Scene *temp = it->second.child;
	//if olready active
	while (actives.size() && actives.top() == uid)
		pop();
	//disable activation
	while (actives.contains(uid))
		actives.erase(uid);
	//delete from map
	scenes.erase(it);
	//return
	return temp;
}
//physics and render attributes deserialize
void Scene::metaDataDeserialize(const Table& table)
{
	//world
	World::deserialize(table);
	//render
	Render::deserialize(table);
}
//physics and render attributes serialize
void Scene::metaDataSerialize(Table& table)
{
	//world
	World::serialize(table);
	//render
	Render::serialize(table);
}
//deserialize
void Scene::deserialize(const Table& table)
{
	//world
	World::deserialize(table);
	//render
	Render::deserialize(table);
	//objects
	const Table& objects = table.getConstTable("Objects");
	//load objects
	for (auto it : objects)
	{
		if (it.second->asType(Table::TABLE))
		{
			//get table
			const Table& table = it.second->get<Table>();
			DEBUG_ASSERT(it.first.isString());
			addObject(new Object())->deserialize(table);
		}
	}
}
//serialize
void Scene::serialize(Table& table)
{
	//world
	World::serialize(table);
	//render
	Render::serialize(table);
	//objects
	Table& tableObjects = table.createTable("Objects");
	//all objects
	for (auto it : objects)
	{
		DEBUG_ASSERT(it->getName() != ""); 
		Table& objTable = tableObjects.createTable("Objects");
		it->serialize(objTable);
	}
}
//utility methos
Easy2D::Screen* Scene::getScreen()
{
    return Application::instance()->getScreen();
}
Easy2D::Audio* Scene::getAudio()
{
    return Application::instance()->getAudio();
}
Easy2D::Input* Scene::getInput()
{
    return Application::instance()->getInput();
}
Easy2D::Game* Scene::getGame()
{
    return Application::instance()->getGame();
}
Easy2D::ResourcesGroup* Scene::getResourcesGroup(const String& name)
{
    return Application::instance()->getResourcesGroup(name);
}