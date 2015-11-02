#ifndef COMPONENT_H
#define COMPONENT_H

#include <Config.h>
#include <EString.h>
#include <Table.h>

namespace Easy2D {
    /////////////////////
    class Scene;
    class Object;
    class Component;
    class ComponentMap;
    template<class T>
    class ComponentItem;
    /////////////////////
    class Component : public AlignedAlloc<16>
    {

        //friend class
        friend class Object;
        friend class ComponentMap;
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
    
        virtual ~Component()
        {
        }
        
        bool isAdded()
        {
            return object!=nullptr;
        }   

		Object* getObject();
		Scene* getScene();
		const Object* getObject() const;
		const Scene* getScene() const;
        //event: object
        virtual void onSetObject(Object* obj){}
        virtual void onChangedMatrix(){}
        virtual void onEraseObject(){}
        //event: scene
		virtual void onSetScene(Scene* scene){}
		virtual void onScenePause(){}
		virtual void onSceneResume(){}
        virtual void onEraseScene(){}
        //event: message
        virtual void onMessage(uint message){}
        //event: run
        virtual void onRun(float dt){}
        //event: run post logic
        virtual void onFixedRun(float dt){}
        //get name
		virtual const char* getComponentName() const{ return  "Component"; };
		virtual uint  getComponentFamily() const{ return 0; };
        //serialize methos
        virtual void serialize(Table& table){}
        virtual void deserialize(const Table& table){}

	};
    /////////////////////
    class ComponentMap
    {
        //components
        typedef Component* (*createComponent)();
        static DUNORDERED_MAP<String,createComponent>* cmap;
        static DUNORDERED_MAP<String,uint>* fmap;
        
        public:
        
        static Component* create(const String& name);
        static uint getFamily(const String& name);
        static void append(const String& name,createComponent fun,uint family);
        //family
        static uint newFamilyID()
        {
            static uint value=0;
            return ++value;
        }
        
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
            ComponentMap::append(name,
                                 ComponentItem<T>::create,
                                 T::Family());
        }

    public:
        

        static ComponentItem<T>& Instance(const std::string& name)
        {
            static ComponentItem<T> componentItem(name);
            return componentItem;
        }
        
    };
    #define REGISTERED_COMPONENT(classname,name)\
    namespace\
    {\
    static const ComponentItem<classname>& _Easy2D_ ## classname ## _ComponentItem=\
                                           ComponentItem<classname>::Instance(name);\
    }

    #define DERIVATE_COMPONENT(Name)\
        virtual const char* getComponentName() const{ return #Name ; };

    #define ADD_COMPONENT_METHOS(Name)\
        virtual const char*  getComponentName() const { return #Name ; };\
        virtual uint getComponentFamily() const { return Family() ; };\
        static  uint Family(){ static uint id=ComponentMap::newFamilyID() ; return id ; };
    /////////////////////

    
};

#endif