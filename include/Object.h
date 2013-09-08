#ifndef OBJECT_H
#define OBJECT_H

#include <Config.h>
#include <Math3D.h>

namespace Easy2D{
	
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
		void addChild(Object *child,ParentMode type=ENABLE_ALL);
		void erseChild(Object *child);
		void changeParentMode(ParentMode type);
		//
		Vector2D getScale(bool global=false);
		Vector2D getPosition(bool global=false);
		float getRotation(bool global=false);
		ParentMode getParentMode() const;
		//math
		void change();
		const Matrix4x4& getGlobalMatrix();
		//for each methods
		std::list<Object*>::iterator begin();
		std::list<Object*>::iterator end();
		std::list<Object*>::reverse_iterator rbegin();
		std::list<Object*>::reverse_iterator rend();
		//data
		void *data;

	private:
		
		struct Transform2D{
			//
			Transform2D():alpha(0),scale(1.0f,1.0f){}
			//values
			Vector2D position;
			float alpha;
			Vector2D scale;
			//cast
			operator float*() { return &position.x; }
			operator const float*() const { return &position.x; }
		};
		//local
		Transform2D transform;
		//global
        Matrix4x4 globalMat;
		bool changeValue;
		//childs
		std::list<Object*> childs;
		//parent
		ParentMode parentMode;
		Object* parent;
		Vector2D   getGlobalParentScale();
		//
	};



};

#endif