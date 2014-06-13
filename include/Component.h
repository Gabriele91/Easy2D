#ifndef COMPONENT_H
#define COMPONENT_H

namespace Easy2D {
    /////////////////////
    class Scene;
    class Object;
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
        //get name
        virtual const char* getComponentName() const=0;

	};

};

#endif