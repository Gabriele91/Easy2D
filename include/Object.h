#ifndef OBJECT_H
#define OBJECT_H

#include <Config.h>
#include <Math3D.h>
#include <Body.h>

namespace Easy2D{
	
    //game object
	class Object
	{
	public:
		
		enum ParentMode{
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
		//for each methods
		std::list<Object*>::iterator begin();
		std::list<Object*>::iterator end();
		std::list<Object*>::const_iterator begin() const;
		std::list<Object*>::const_iterator end() const;

		std::list<Object*>::reverse_iterator rbegin();
		std::list<Object*>::reverse_iterator rend();
		std::list<Object*>::const_reverse_iterator rbegin() const;
		std::list<Object*>::const_reverse_iterator rend() const;
        //physics
        Body physics;

	protected:

		//data
		void *data;

	private:
		
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
        
	};



};

#endif