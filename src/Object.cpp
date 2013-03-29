#include <Object.h>
/* Easy2D */
using namespace Easy2D;
//////////////////////
Object::Object()
			   :data(NULL),
		        parentMode(DISABLE_PARENT),
		        parent(NULL),
				changeValue(false){}
Object::~Object(){
	for(auto obj : *this){
		obj->parent=NULL;
		delete obj;
	}
	childs.clear();
}

void Object::setScale(const Vector2D &scale,bool global){
	if(!global||!parent)
		transform.scale=scale;
	else{
		//transform.scale=scale*(transform.scale/getGlobalMatrix().getScale2D());
		transform.scale/=getGlobalMatrix().getScale2D();
		transform.scale*=scale;
	}
	change();
}
void Object::setPosition(const Vector2D &position,bool global){
	if(!global||!parent)
		transform.position=position;
	else
		//transform.position=position+(transform.position-getGlobalMatrix().getTranslation2D());
		transform.position-=getGlobalMatrix().getTranslation2D()-position;

	change();
}
void Object::setRotation(float alpha,bool global){
	if(!global||!parent)
		transform.alpha=alpha;	
	else
		transform.alpha=alpha-getGlobalMatrix().getRotZ();
	change();
}
void Object::setMove(const Vector2D &velocity){
	//
	float lcosf=std::cosf(transform.alpha);
	float lsinf=std::sinf(transform.alpha);
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
void Object::addChild(Object *child,ParentMode type){

	if(child->parent==this) return;
	if(child==this) return;
	if(child->parent) child->parent->erseChild(child);

	child->parent=this;
	child->parentMode=type;
	this->childs.push_back(child);

}
void Object::erseChild(Object *child){
	if(child->parent==this){
		childs.remove(child);
		child->parent=NULL;
	}
}
//
std::list<Object*>::iterator Object::begin(){ return childs.begin(); }
std::list<Object*>::iterator Object::end(){ return childs.begin(); }
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

				case ENABLE_ALL: //ENABLE_ALL //inutile
				case 3: //ENABLE_POSITION+ENABLE_ROTATION
					globalMat.setFastTransform2DTR(transform);
					mtmp.setRotZ(mtmp.getRotZ());
					mtmp.entries[12]=tmpPos.x; mtmp.entries[13]=tmpPos.y;
					globalMat=mtmp.mul2D(globalMat);
				break;

				case ENABLE_POSITION:
					globalMat.setRotZ(transform.alpha);
					globalMat.entries[12]=mtmp.entries[12]+this->transform.position.x;
					globalMat.entries[13]=mtmp.entries[13]+this->transform.position.y;
				break;

				case ENABLE_ROTATION:
					globalMat.setRotZ(mtmp.getRotZ()+this->transform.alpha);
					globalMat.entries[12]=this->transform.position.x;
					globalMat.entries[13]=this->transform.position.y;
				break;
				//VC9
				default:break;
			}

			if(ENABLE_SCALE & parentMode){
			   const Vector2D& vTScale=getGlobalParentScale();
			   mtmp.setScale(vTScale);
			   globalMat=globalMat.mul2D(mtmp);
			}else{
			   mtmp.setScale(transform.scale);
			   globalMat=globalMat.mul2D(mtmp);
			}

		}else
			globalMat.setFastTransform2DTRS(transform);
		//
		changeValue=false;
	 }

return globalMat;
}
Vector2D  Object::getGlobalParentScale(){
           //no steck over flu....
           Object *p=NULL;
           Vector2D out=transform.scale;
           for(p=this->parent; p; p=p->parent ){
                out*=p->transform.scale;
           }
           return out;
}

