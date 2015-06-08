#ifndef OBJECT_H
#define OBJECT_H

#include <Config.h>
#include <Math3D.h>
#include <Body.h>
#include <Table.h>
#include <Component.h>
#include <Debug.h>

namespace Easy2D
{
//dec class
class World;
class Scene;
class Screen;
class Input;
class Audio;
class Game;
class ResourcesGroup;
//game object
class Object
{
    public:
                
    typedef std::map<uint, Component*> Components;
    typedef std::list<Object*> Childs;

    enum ParentMode
    {
        DISABLE_PARENT=0,
        ENABLE_POSITION=1, //1
        ENABLE_ROTATION=2, //10
        ENABLE_SCALE=4,	   //100
        ENABLE_POSITION_ROTATION=ENABLE_POSITION|ENABLE_ROTATION, //11
        ENABLE_ALL=ENABLE_POSITION|ENABLE_ROTATION|ENABLE_SCALE //111
    };
    
    public:

    Object();
    Object(const String& name);
    virtual ~Object();
    //methods
    void setScale(const Vector2D &scale,bool global=false);
    void setPosition(const Vector2D &position,bool global=false);
    void setRotation(Angle alpha,bool global=false);
    //
    void setMove(const Vector2D &velocity);
    void setTranslation(const Vector2D &translation);
    void setTurn(Angle alpha);
    //setTransform
    void setTransform(const Transform2D& tr2d)
    {
        if(transform!=tr2d)
        {
            transform=tr2d;
            change();
        }
    }
    void forceTransform(const Transform2D& tr2d,bool global=false);
    //childs
    bool hasChild(Object *child) const;
    bool hasChild(const String& name) const;
    Object* addChild(Object *child,bool ptrdelete=true);
    Object* addChild(Object *child,int parentMode,bool ptrdelete=true);
    void eraseChild(Object *child);
    void setParentMode(int type);
    void freeChilds();
    size_t countChilds() const;
    size_t depthChilds() const;
    //
    Vector2D getScale(bool global=false);
    Vector2D getPosition(bool global=false);
    Angle    getRotation(bool global=false);
    int getParentMode() const;
    Object* getParent();
    const Object* getParent() const;

    //set/get z
    void setZ(int z,bool global=false);
    int  getZ(bool global=false);
    void changeZ();

    //math
    void change();
    virtual const Matrix4x4& getGlobalMatrix();

    //scene
    Scene* getScene()
    {
        return scene;
    }
    const Scene* getScene() const
    {
        return scene;
    }

    //for each methods
    Childs::iterator begin();
    Childs::iterator end();
    Childs::const_iterator begin() const;
    Childs::const_iterator end() const;

    Childs::reverse_iterator rbegin();
    Childs::reverse_iterator rend();
    Childs::const_reverse_iterator rbegin() const;
    Childs::const_reverse_iterator rend() const;
    
    //components by name
    Component* component(const String& name);
    //components list
    const Components& getComponents();
    //get a component
	void addComponent(Component* component)
	{
		if (component != nullptr)
		{
			if (!component->isAdded() && components.find(component->getComponentFamily()) == components.end())
			{
				components[component->getComponentFamily()] = component;
				component->setEntity(this);
			}
			else
			{
				DEBUG_MESSAGE(component->getComponentName() << " component olready edded");
			}
		}
		else
		{
			DEBUG_MESSAGE("isn't a component");
		}
	}
    Component* getComponent(const String& name)
    {
        auto it=components.find(ComponentMap::getFamily(name));
        if(it==components.end()) return nullptr;
        return it->second;
    }
    const Component* getComponent(const String& name) const
    {
        auto it=components.find(ComponentMap::getFamily(name));
        if(it==components.end()) return nullptr;
        return it->second;
    }
    Component* removeComponent(const String& name)
    {
        auto itc=components.find(ComponentMap::getFamily(name));
        if(itc!=components.end())
        {
            auto component=itc->second;
            
            components.erase(itc);
            
            component->removeEntity();
            
            return component;
        }
        return nullptr;
    }
    bool hasComponent(const String& name)
    {
        auto it=components.find(ComponentMap::getFamily(name));
        if(it==components.end()) return false;
        return true;
    }
    //components by template
    template<class T>
    void addComponent(T* component)
    {
        Component* newcmp=dynamic_cast< Component* >(component);
        if(newcmp!=nullptr)
        {
            if(!component->isAdded() && components.find(T::Family())==components.end())
            {
                components[T::Family()]=component;
                component->setEntity(this);
            }
            else 
            {
                DEBUG_MESSAGE(component->getComponentName()<<" component olready edded");
            }

        }
        else
        {
            DEBUG_MESSAGE("isn't a component");
        }
    } 
    template<class T>
    T* getComponent()
    {
        auto itc=components.find(T::Family());

        if(itc!=components.end())
        {
            return dynamic_cast<T*>(itc->second);
        }

        return nullptr;
    }    
    template<class T>
    const T* getComponent() const
    {
        auto itc=components.find(T::Family());

        if(itc!=components.end())
        {
            return dynamic_cast<T*>(itc->second);
        }

        return nullptr;
    }
    template<class T>
    T* removeComponent()
    {
        auto itc=components.find(T::Family());
        if(itc!=components.end())
        {
            auto component=itc->second;
            
            components.erase(itc);
            
            component->removeEntity();
            
            return component;
        }
        return nullptr;
    }
    template<class T> 
    bool hasComponent()
    {
        auto itc=components.find(T::Family());

        if(itc!=components.end())
        {
            return true;
        }

        return false;
    }

    //send a message
    void sendMessage(uint message);
    void sendLocalMessage(uint message);
    
    //object callbacks 
	virtual void onResume(){ }
	virtual void onRun(float dt){ }
	virtual void onPause(){ }
    virtual void onAttached(Scene* scene){ }
    virtual void onUnattached(){ }
    
    void  setUserData(void* data)
    {
		this->userData = data;
    }
    void* getUserData() const 
    {
		return userData;
    }   
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
    //query info
    Object* getObject(const String& argname);
    Object* getChild(const String& argname);
    void setName(const String& argname);
    const String& getName();
    
    protected:

    //utility methos
    Screen* getScreen();
    Audio* getAudio();
    Input* getInput();
    Game* getGame();
    ResourcesGroup* getResourcesGroup(const String& name);
	//data
	mutable void* userData;

    private:
    //find child
    Object* getPrivateObject(const std::vector<String>& names,size_t i);
    //name
    String name;
    //scene
    Scene*  scene;
    //components
    Components components;
    //append a scene
    void setScene(Scene* argscene);
    void eraseScene();
    //scene run
	void onScenePause();
	void onSceneRun(float dt);
	void onSceneResume();
    //local
    Transform2D transform;
    //global
    bool changeValue;
    Matrix4x4 globalMat;
    //float z
    int zLocal;
    int zGlobal;
    bool changeZValue;
    //parent
    bool    del;
    Object* parent;
    int parentMode;
    void computeMatrix(const Transform2D& transform,Matrix4x4& globalMat) const;
    Vector2D   getGlobalParentScale() const;
    //childs
    std::list<Object*> childs;
    //friend class
    friend class Body;
    friend class World;
    friend class Scene;
    

};



};

#endif