#ifndef OBJECT_H
#define OBJECT_H

#include <Config.h>
#include <Math3D.h>
#include <Body.h>
#include <Component.h>

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

    enum ParentMode
    {
        DISABLE_PARENT=0,
        ENABLE_POSITION=1, //1
        ENABLE_ROTATION=2, //10
        ENABLE_SCALE=4,	   //100
        ENABLE_ALL=ENABLE_POSITION|ENABLE_ROTATION|ENABLE_SCALE //111
    };

    Object();
    virtual ~Object();
    //methods
    void setScale(const Vector2D &scale,bool global=false);
    void setPosition(const Vector2D &position,bool global=false);
    void setRotation(float alpha,bool global=false);
    //
    void setMove(const Vector2D &velocity);
    void setTranslation(const Vector2D &translation);
    void setTurn(float alpha);
    //setTransform
    void setTransform(const Transform2D& tr2d)
    {
        if(transform!=tr2d)
        {
            transform=tr2d;
            change();
        }
    }
    //childs
    void addChild(Object *child,bool ptrdelete=true);
    void addChild(Object *child,ParentMode type,bool ptrdelete=true);
    void erseChild(Object *child);
    void changeParentMode(ParentMode type);
    //
    Vector2D getScale(bool global=false);
    Vector2D getPosition(bool global=false);
    float getRotation(bool global=false);
    ParentMode getParentMode() const;
    //math
    void change();
    virtual const Matrix4x4& getGlobalMatrix();
    Scene* getScene()
    {
        return scene;
    }
    const Scene* getScene() const
    {
        return scene;
    }

    //for each methods
    std::list<Object*>::iterator begin();
    std::list<Object*>::iterator end();
    std::list<Object*>::const_iterator begin() const;
    std::list<Object*>::const_iterator end() const;

    std::list<Object*>::reverse_iterator rbegin();
    std::list<Object*>::reverse_iterator rend();
    std::list<Object*>::const_reverse_iterator rbegin() const;
    std::list<Object*>::const_reverse_iterator rend() const;
    
    //utility methos
    Screen* getScreen();
    Audio* getAudio();
    Input* getInput();
    Game* getGame();
    ResourcesGroup* getResourcesGroup(const String& name);

    //components
    template<class T> 
    void addComponent(T* component)
    {
        Component* newcmp=dynamic_cast< Component* >(component);
        if(newcmp!=nullptr)
        {
            if(!component->isAdded() && components.find(T::getComponentType())==components.end())
            {
                components[T::getComponentType()]=component;
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
        auto itc=components.find(T::getComponentType());

        if(itc!=components.end())
        {
            return dynamic_cast<T*>(itc->second);
        }

        return nullptr;
    }    
    template<class T>
    const T* getComponent() const
    {
        auto itc=components.find(T::getComponentType());

        if(itc!=components.end())
        {
            return dynamic_cast<T*>(itc->second);
        }

        return nullptr;
    }
    template<class T>
    void removeComponent()
    {
        auto itc=components.find(T::getComponentType());
        if(itc!=components.end)
        {
            components.erase(itc);
            component->removeEntity();
        }
    }
    template<class T> 
    void hasComponent()
    {
        auto itc=components.find(T::getComponentType());

        if(itc!=components.end())
        {
            return true;
        }

        return false;
    }
    //send a message
    void sendMessage(uint message)
    {
        //message to components
        for(auto cmp:components)
        {
            cmp.second->onMessage(message);
        }
        //message to childs
        for(auto child:childs)
        {
            child->sendMessage(message);
        }
    }
    
    //object call backs 
    //update
    virtual void onRun(float dt)
    {
        
    }
    virtual void onAttached(Scene* scene)
    {
        
    }
    virtual void onUnattached()
    {
        
    }
    
    void  setUserData(void* data)
    {
        this->data=data;
    }
    void* getUserData()
    {
        return data;
    }
    
    private:
    //data
    void*   data;
    //scene
    Scene*  scene;
    //components
    std::map<const  type_info* , Component* > components;
    //append a scene
    void setScene(Scene* argscene)
    {
        //DEBUG_ASSERT(!scene);
        scene=argscene;
        //call back
        onAttached(scene);
        //message to components
        for(auto cmp:components)
        {
            cmp.second->onSetScene(scene);
        }
        //message to childs
        for(auto child:childs)
        {
            child->setScene(scene);
        }
    }
    void eraseScene()
    {
        //DEBUG_ASSERT(scene);
        //message to components
        for(auto cmp:components)
        {
            cmp.second->onEraseScene();
        }
        //message to childs
        for(auto child:childs)
        {
            child->eraseScene();
        }
        //call back
        onUnattached();
        //dt
        scene=nullptr;
    }
    //scene run
    void onSceneRun(float dt)
    {
        //update components
        for(auto cmp:components)
        {
            cmp.second->onRun(dt);
        }
        //update 
        onRun(dt);
        //update childs
        for(auto child:childs)
        {
            child->onSceneRun(dt);
        }
    }
    //local
    Transform2D transform;
    //global
    bool changeValue;
    Matrix4x4 globalMat;
    //parent
    bool    del;
    Object* parent;
    ParentMode parentMode;
    Vector2D   getGlobalParentScale();
    //childs
    std::list<Object*> childs;
    //friend class
    friend class Body;
    friend class World;
    friend class Scene;
    

};



};

#endif