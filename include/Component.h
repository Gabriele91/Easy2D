#ifndef COMPONENT_H
#define COMPONENT_H

#include <Config.h>
#include <EString.h>
#include <Table.h>

namespace Easy2D {
    /////////////////////
    class Scene;
    class Object;
    class ComponentMap;
    /////////////////////
	class Component
    {

        //friend class
        friend class Object;
        //private info
        Object* object;

        inline void setEntity(Object* obj)
        {
            object=obj;
            onSetObject(object);
        }
        inline void removeEntity()
        {
            object=nullptr;
        }
    
        public:

        Component():object(nullptr)
        {
        }
    
        bool isAdded()
        {
            return object!=nullptr;
        }   

        Object* getObject()
        {
            return object;
        }   
        //event: object
        virtual void onSetObject(Object* obj){}
        virtual void onEraseObject(){};
        //event: scene
        virtual void onSetScene(Scene* scene){}
        virtual void onEraseScene(){}
        //event: message
        virtual void onMessage(uint message){}
        //event: run
        virtual void onRun(float dt){}
        //event: run post logic
        virtual void onFixedRun(float dt){}
        //get name
        virtual const char* getComponentName() const=0;
        //get info
        virtual const cppTypeInfo* getComponentInfo() const=0;
        //serialize methos
        virtual void serialize(Table& table){}
        virtual void deserialize(const Table& table){}

	};
    /////////////////////
    class ComponentMap
    {
        
        typedef Component* (*createComponent)();
        static DUNORDERED_MAP<String,createComponent>* cmap;
        
        public:
        
        static Component* create(const String& name);
        static void append(const String& name,createComponent);
        
    };
    /////////////////////
    template<class T>
    class ComponentItem
    {
        
        static Component* create()
        {
            return (Component*)new T();
        }
        
        ComponentItem(const String& name)
        {
            ComponentMap::append(name,ComponentItem<T>::create);
        }

    public:
        

        static ComponentItem<T>& Instance(const std::string& name)
        {
            static ComponentItem<T> componentItem(name);
            return componentItem;
        }
        
    };
    #define REGISTERED_COMPONENT(classname,name)\
    namespace{  static const ComponentItem<classname>& _Easy2D_ ## classname ## _ComponentItem=ComponentItem<classname>::Instance(name);}

    #define DERIVATE_COMPONENT(T)\
        virtual const char* getComponentName() const{ return #T ; };

    #define ADD_COMPONENT_METHOS(T)\
        DERIVATE_COMPONENT(T)\
        virtual const cppTypeInfo* getComponentInfo() const{ return &typeid(T) ; };\
        static const  cppTypeInfo* getComponentType(){ return &typeid(T) ; };
    /////////////////////

    
};

#endif