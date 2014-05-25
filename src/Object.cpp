#include <stdafx.h>
#include <Object.h>
/* Easy2D */
using namespace Easy2D;
//////////////////////
Object::Object()
			   :data(NULL),
				changeValue(false),
				del(false),
		        parent(NULL),
		        parentMode(DISABLE_PARENT){}
Object::~Object(){
	for(auto obj : *this){
		obj->parent=NULL;
		if(obj->del)
			delete obj;
		else
			obj->change();
	}
	childs.clear();
}

void Object::setScale(const Vector2D &scale,bool global){
	if(!global||!parent)
		transform.scale=scale;
	else{
		transform.scale=scale/parent->getScale(true);
	}
	change();
}
void Object::setPosition(const Vector2D &position,bool global){
	if(!global||!parent)
		transform.position=position;
	else
		transform.position=position-parent->getPosition(true);

	change();
}
void Object::setRotation(float alpha,bool global){
	if(!global||!parent)
		transform.alpha=alpha;
	else
		transform.alpha=alpha-parent->getRotation(true);
	change();
}
void Object::setMove(const Vector2D &velocity){
	//
	float lcosf=std::cos(transform.alpha);
	float lsinf=std::sin(transform.alpha);
	//like openGL
	transform.position.x+= velocity.x*lcosf+velocity.y*lsinf;
	transform.position.y+=-velocity.x*lsinf+velocity.y*lcosf;
	//
	change();
}
void Object::setTranslation(const Vector2D &translation){
	transform.position+=translation;
	change();
}
void Object::setTurn(float alpha){
	transform.alpha+=alpha;
	change();
}
//
Vector2D Object::getScale(bool global){
	if(!global||!parent)
		return transform.scale;
	else
		return getGlobalMatrix().getScale2D();
}
Vector2D Object::getPosition(bool global){
	if(!global||!parent)
		return transform.position;
	else
		return getGlobalMatrix().getTranslation2D();
}
float Object::getRotation(bool global){
	if(!global||!parent)
		return transform.alpha;
	else
		return getGlobalMatrix().getRotZ();
}
Object::ParentMode Object::getParentMode() const{
	return parentMode;
}
//
void Object::change(){
	if(!changeValue){
		for(auto obj : *this ){
		  obj->changeValue=true;
		  obj->change();
		}
		changeValue=true;
	}
}
void Object::addChild(Object *child,bool ptrdelete){
	addChild(child,ParentMode::ENABLE_ALL,ptrdelete);
}
void Object::addChild(Object *child,ParentMode type,bool ptrdelete){

	if(child->parent==this) return;
	if(child==this) return;
	if(child->parent) child->parent->erseChild(child);
	
	child->del=ptrdelete;
	child->parent=this;
	child->parentMode=type;
	this->childs.push_back(child);

	child->change();

}
void Object::erseChild(Object *child){
	if(child->parent==this){
		childs.remove(child);
		child->parent=NULL;
		child->change();
	}
}
//
std::list<Object*>::iterator Object::begin(){ return childs.begin(); }
std::list<Object*>::iterator Object::end(){ return childs.end(); }
std::list<Object*>::const_iterator Object::begin() const { return childs.begin(); }
std::list<Object*>::const_iterator Object::end() const { return childs.end(); }

std::list<Object*>::reverse_iterator Object::rbegin() { return childs.rbegin(); }
std::list<Object*>::reverse_iterator Object::rend() { return childs.rend(); }
std::list<Object*>::const_reverse_iterator Object::rbegin() const { return childs.rbegin(); }
std::list<Object*>::const_reverse_iterator Object::rend() const { return childs.rend(); }
//
const Matrix4x4& Object::getGlobalMatrix(){
	if(changeValue==true){
		//
		globalMat.identity();
		//
		if(parent){
			Matrix4x4 mtmp;
			mtmp=parent->getGlobalMatrix();
			Vector2D tmpPos(mtmp.entries[12],mtmp.entries[13]);

			switch(parentMode & (ENABLE_POSITION | ENABLE_ROTATION)){

				case ENABLE_ALL: 
				case 3:			 //ENABLE_POSITION+ENABLE_ROTATION
					globalMat.setTransform2D(transform);
					mtmp.setRotZ(mtmp.getRotZ());
					mtmp.entries[12]=tmpPos.x; 
					mtmp.entries[13]=tmpPos.y;
					globalMat=mtmp.mul2D(globalMat);
				break;

				case ENABLE_POSITION:
					globalMat.setRotZ(transform.alpha);
					globalMat.entries[12]=tmpPos.x+transform.position.x;
					globalMat.entries[13]=tmpPos.y+transform.position.y;
				break;

				case ENABLE_ROTATION:
					globalMat.setRotZ(mtmp.getRotZ()+transform.alpha);
					globalMat.entries[12]=transform.position.x;
					globalMat.entries[13]=transform.position.y;
				break;
				//VC9
				default:break;
			}

			if(ENABLE_SCALE & parentMode)
				globalMat.addScale(getGlobalParentScale());
			else
				globalMat.addScale(transform.scale);
			

		}else
			globalMat.setTransform2D(transform);
		//
		changeValue=false;
	 }

return globalMat;
}
Vector2D  Object::getGlobalParentScale(){
           Object *p=NULL;
           Vector2D out=transform.scale;
           for(p=this->parent; p; p=p->parent ){
			   if(ENABLE_SCALE & p->parentMode)
					out*=p->transform.scale;
			   else
					out=p->transform.scale;
           }
           return out;
}

