#include <stdafx.h>
#include <Object.h>
#include <Body.h>
#include <Scene.h>
#include <Debug.h>
#include <Application.h>
/* Easy2D */
using namespace Easy2D;
//////////////////////
Object::Object()
	:userData(nullptr)
    ,scene(nullptr)
    ,changeValue(false)
    ,zLocal(0)
    ,zGlobal(0)
    ,changeZValue(false)
    ,del(false)
    ,parent(nullptr)
    ,parentMode(DISABLE_PARENT) 
{
}
Object::Object(const String& argname)
	:userData(nullptr)
    ,scene(nullptr)
    ,changeValue(false)
    ,zLocal(0)
    ,zGlobal(0)
    ,changeZValue(false)
    ,del(false)
    ,parent(nullptr)
    ,parentMode(DISABLE_PARENT)
    ,name(argname)
{
}
Object::~Object()
{
    //destory childs
    freeChilds();
    //destroy components
    for(auto cmp:components)
        delete cmp.second;

}

void Object::setScale(const Vector2D &scale,bool global)
{
    if(!global||!parent || !(parentMode & ENABLE_SCALE))
        transform.scale=scale;
    else
    {
        transform.scale=scale/getGlobalParentScale();
    }
    change();
}
void Object::setPosition(const Vector2D &position,bool global)
{
    if( !global || !parent || !(parentMode & ENABLE_POSITION))
        transform.position=position;
    else
    {
        
        Vector2D newposition;
        
        switch (parentMode)
        {
            //rotation
            default:
            {
                //get word position
                Mat4 wordPos;
                Transform2D toWord;
                computeMatrix(toWord, wordPos);
                wordPos.inverse();
                newposition=wordPos.mul2D(position);
                
                if(!(parentMode & ENABLE_SCALE))
                    newposition/=getGlobalParentScale();
            }
            break;
            //no rotation
            case ENABLE_POSITION:
            case ENABLE_POSITION|ENABLE_SCALE:
            {
                Mat4 pRotation;
                pRotation.setRotZ(Angle(Radian(Math::PI2))- parent->getGlobalMatrix().getRotZ());
                newposition  = pRotation.mul2D( position  - parent->getPosition(true))  / getGlobalParentScale();
            }
            break;
        }
        
        transform.position=newposition;

    }
    change();
}
void Object::setRotation(Angle alpha,bool global)
{
    if(!global||!parent || !(parentMode & ENABLE_ROTATION))
        transform.alpha=alpha;
    else
        transform.alpha=alpha-parent->getRotation(true);
    change();
}
void Object::setMove(const Vector2D &velocity)
{
    //var dec
    Mat4 mrot;
    //set rot
    mrot.setRotZ(transform.alpha);
    //calc dir
    transform.position+=mrot.mul2D(velocity);
    //send info
    change();
}
void Object::setTranslation(const Vector2D &translation)
{
    transform.position+=translation;
    change();
}
void Object::setTurn(Angle alpha)
{
    transform.alpha+=alpha;
    change();
}
void Object::setParentMode(int type)
{
    if(parentMode!=type)
    {
        parentMode=type;
        change();
    }
}
//
Vector2D Object::getScale(bool global)
{
    if(!global||!parent || !(parentMode & ENABLE_SCALE))
        return transform.scale;
    else
        return getGlobalParentScale()*transform.scale;
}
Vector2D Object::getPosition(bool global)
{
    if(!global||!parent || !(parentMode & ENABLE_POSITION))
        return transform.position;
    else
        return getGlobalMatrix().getTranslation2D();
}
inline Angle rotation2D(const Mat4& m4,const Vec2& scale)
{
    float a=m4.m00;
    float b=m4.m01;
    if(a<=0.00001 && a>=-0.00001) a=0.0;
    if(b<=0.00001 && b>=-0.00001) b=0.0;
    //get angle
    float angle=std::atan2(b,a);

    if(scale.x<0.0)
    {
        angle+=Math::PI;
    }
    
    return Angle(Radian(angle));
}
Angle Object::getRotation(bool global)
{
    if(!global||!parent || !(parentMode & ENABLE_ROTATION))
        return transform.alpha;
    else
        return rotation2D(getGlobalMatrix(),getScale(true));
        //return Math::todeg(getGlobalMatrix().getRotZ());
}
int Object::getParentMode() const
{
    return parentMode;
}
Object* Object::getParent() 
{
    return parent;
}
const Object* Object::getParent() const
{
    return parent;
}
//
void Object::setZ(int z,bool global)
{
    if(!global||!parent)
        zLocal=z;
    else 
        zLocal=z-parent->getZ(true);
    changeZ();
}
int Object::getZ(bool global)
{
    if(!global||!parent)
        return zLocal;
    else 
    {
        if(changeZValue)
        {
            zGlobal=parent->getZ(true)+zLocal;
            changeZValue=false;
        }
        return zGlobal;
    }
}
void Object::changeZ()
{
    if(!changeZValue)
    {
        for(auto obj : *this )
        {
            obj->changeZ();
        }
        changeZValue=true;
    }
}
//CHILDS
bool Object::hasChild(Object *child) const
{
    return std::find(begin(),end(),child)!=end();
}
bool Object::hasChild(const String& name) const
{
    for(auto child:*this) 
        if(child->name==name) 
            return true;
    return false;
}
void Object::change()
{
    if(!changeValue)
    {
        for(auto obj : *this )
        {
			if (obj->getParentMode() != DISABLE_PARENT)
			{
				obj->change();
			}
        }
        changeValue=true;
    }
}
Object* Object::addChild(Object *child,bool ptrdelete)
{
    int mode=ParentMode::ENABLE_ALL;
    if(child->parent) mode=child->getParentMode();
    return addChild(child,mode,ptrdelete);
}
Object* Object::addChild(Object *child,int parentMode,bool ptrdelete)
{
    
    if(child==this) return nullptr;
    if(child->parent==this) return child;
    if(child->parent) child->parent->eraseChild(child);

    child->del=ptrdelete;
    child->parent=this;
    child->parentMode=parentMode;
    //push
    this->childs.push_back(child);
	if (parentMode != DISABLE_PARENT)
		child->change();
    //event
    if(getScene())
        child->setScene(getScene());
    
    return child;
}
void Object::eraseChild(Object *child)
{
    if(child->parent==this)
    {
        //events
        child->eraseScene();
        child->parent=NULL;
        //remove
        childs.remove(child);
        //send
        child->change();
    }
}
void Object::freeChilds()
{
    //destory childs
    for(auto obj : *this)
    {
        obj->parent=NULL;
        if(obj->del)
            delete obj;
        else
            obj->change();
    }
    childs.clear();
}
size_t Object::countChilds() const
{
    return childs.size();
}
size_t Object::depthChilds() const
{
    size_t depth=0;
    for(auto achild:childs)
    {
        depth=Math::max(depth,achild->depthChilds());
    }

    return depth+1;
}
//
Object::Childs::iterator Object::begin()
{
    return childs.begin();
}
Object::Childs::iterator Object::end()
{
    return childs.end();
}
Object::Childs::const_iterator Object::begin() const
{
    return childs.begin();
}
Object::Childs::const_iterator Object::end() const
{
    return childs.end();
}

Object::Childs::reverse_iterator Object::rbegin()
{
    return childs.rbegin();
}
Object::Childs::reverse_iterator Object::rend()
{
    return childs.rend();
}
Object::Childs::const_reverse_iterator Object::rbegin() const
{
    return childs.rbegin();
}
Object::Childs::const_reverse_iterator Object::rend() const
{
    return childs.rend();
}

Object* Object::getObject(const String& argname)
{   //name list
    std::vector<String> names;
    argname.split(".",names);
    return getPrivateObject(names,0);
}
Object* Object::getChild(const String& argname)
{   //name list
    std::vector<String> names;
    argname.split(".",names);
    //is 0 return null
    if(!argname.size())
        return nullptr;
    //is >0
    for(auto child:*this)
    {
        Object* tmp=child->getPrivateObject(names,0);
        if(tmp) return tmp;
    };
    //else null
    return nullptr;
}
void Object::setName(const String& argname)
{
    name=argname;
}
const String& Object::getName()
{
    return name;
}
Object* Object::getPrivateObject(const std::vector<String>& names,size_t i)
{
    if(!names.size())
        return nullptr;

    if(names[i]==name) 
    {
        if(names.size()-1==i)
            return this;
        else if(names.size()!=(i+1))
        {
            for(auto child:*this)
            {
                Object* tmp=child->getPrivateObject(names,i+1);
                if(tmp) return tmp;
            };
        }
    }

    return nullptr;
}

//components by name
Component* Object::component(const String& name)
{
    auto cmp=ComponentMap::create(name);
    auto it=components.find(cmp->getComponentFamily());
    if(it==components.end())
    {
        //set entity
        cmp->setEntity(this);
        //set scene
        if(getScene()) cmp->onSetScene(scene);
        //add component
        components[cmp->getComponentFamily()]=cmp;
        //return
        return cmp;
    }
    DEBUG_ASSERT_MSG(0,"component "<<name<<" in "<<getName()<<" olready exist");
    delete cmp;
    return nullptr;
}
//components list
const Object::Components& Object::getComponents()
{
    return components;
}
//send a message
void Object::sendMessage(uint message)
{
    //message to components
    sendLocalMessage(message);
    //message to childs
    for(auto child:childs)
    {
        child->sendMessage(message);
    }
}
void Object::sendLocalMessage(uint message)
{
    //message to components
    for(auto cmp:components)
    {
        cmp.second->onMessage(message);
    }
}
//force transform
void Object::forceTransform(const Transform2D& tr2d,bool global)
{
    transform=tr2d;
    //calc
    if(global)
        globalMat.setTransform2D(transform);
    else
        computeMatrix(transform,globalMat);
    
    //change
    change();
    //only chailds
    changeValue=false;
}
//Matrix
const Matrix4x4& Object::getGlobalMatrix()
{
    if(changeValue==true)
    {
        //update
        changeValue=false;
        //calc
        computeMatrix(transform,globalMat);
        //sending event to components
        for(auto component:components)
            component.second->onChangedMatrix();
    }
    
    return globalMat;
}
void Object::computeMatrix(const Transform2D& transform,Matrix4x4& globalMat) const
{
    //
    //globalMat.identity();
    //
    if(parent)
    {

        switch(parentMode)
        {
            case ENABLE_ALL:
            {
                Mat4 pmodel=parent->getGlobalMatrix();
                globalMat.setTransform2D(transform);
                globalMat=pmodel.mul2D(globalMat);
            }
            break;
            case ENABLE_POSITION | ENABLE_ROTATION:
            {
                Mat4 pmodel=parent->getGlobalMatrix();
                //pivot
                Vec2 pivot = pmodel.mul2D(transform.position);
                //set tranform
                Transform2D local=transform;
                local.position= pivot;
                local.alpha  += pmodel.getRotZ();
                globalMat.setTransform2D(local);

            }
            break;
            case ENABLE_POSITION:
            case ENABLE_POSITION|ENABLE_SCALE:
            {
                Mat4 pmodel=parent->getGlobalMatrix();
                Vec2 pivot=pmodel.mul2D(transform.position);
                Transform2D local=transform;
                local.position=pivot;
                
                if(parentMode&ENABLE_SCALE)
                {
                    local.scale*=getGlobalParentScale();
                }
                
                globalMat.setTransform2D(local);
            }
            break;
            case ENABLE_ROTATION:
            case ENABLE_ROTATION|ENABLE_SCALE:
            {
                Mat4 pmodel=parent->getGlobalMatrix();
                Transform2D local=transform;
                local.alpha     +=pmodel.getRotZ();
                
                if(parentMode&ENABLE_SCALE)
                {
                    local.scale*=getGlobalParentScale();
                }
                
                globalMat.setTransform2D(local);
            }
            break;
            case ENABLE_SCALE:
            {
                Transform2D local=transform;
                
                if(parentMode&ENABLE_SCALE)
                {
                    local.scale*=getGlobalParentScale();
                }
                
                globalMat.setTransform2D(local);
            }
            break;
            case DISABLE_PARENT:
                globalMat.setTransform2D(transform);
            break;
            default:
                DEBUG_ASSERT_MSG(0,"invalid parentMode");
            break;
        }
        
    }
    else
        globalMat.setTransform2D(transform);
    
}

Vector2D  Object::getGlobalParentScale() const
{
    //no parent?
    if(!parent) return Vec2::ONE;
    //calc
    Object *p1=NULL;
    Object *p2=NULL;
    Vector2D out=parent->transform.scale;
    //bottom up
    for(p1=parent,p2=parent->parent; p2 ; p1=p2,p2=p2->parent )
    {
        if(ENABLE_SCALE & p1->parentMode)
            out*=p2->transform.scale;
    }
    return out;
}

//SCENE EVENTS
//append a scene
void Object::setScene(Scene* argscene)
{
    if(scene!=argscene) eraseScene();
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
void Object::eraseScene()
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
void Object::onSceneRun(float dt)
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
    //fixed components
    for(auto cmp:components)
    {
        cmp.second->onFixedRun(dt);
    }
}
void Object::onScenePause()
{
	//update childs
	for (auto child : childs)
	{
		child->onScenePause();
	}
	//update 
	onPause();
	//update components
	for (auto cmp : components)
	{
		cmp.second->onScenePause();
	}
}
void Object::onSceneResume()
{
	//update components
	for (auto cmp : components)
	{
		cmp.second->onSceneResume();
	}
	//update 
	onResume();
	//update childs
	for (auto child : childs)
	{
		child->onSceneResume();
	}
}
//utility methos
Screen* Object::getScreen()
{
    return Application::instance()->get_screen();
}
Audio* Object::getAudio()
{
    return Application::instance()->get_audio();
}
Input* Object::getInput()
{
    return Application::instance()->get_input();
}
Game* Object::getGame()
{
    return Application::instance()->get_game();
}
ResourcesGroup* Object::getResourcesGroup(const String& name)
{
    return Application::instance()->get_resources_group(name);
}

static String parentToString(int mode)
{
    if(mode == Object::DISABLE_PARENT) return "DISABLE";
    if(mode == Object::ENABLE_ALL) return "ALL";

    String smode;
    if(mode & Object::ENABLE_POSITION) smode = "POSITION";
    if(mode & Object::ENABLE_ROTATION) smode+=String(smode.size() ? "|" : "") + "ROTATION";
    if(mode & Object::ENABLE_SCALE)    smode+=String(smode.size() ? "|" : "") + "SCALE";

    return smode;
}
static int stringToParent(const String& argsmode)
{
    if(!argsmode.size()) return Object::DISABLE_PARENT;
    //normalize
    String smode=argsmode.toUpper();
    smode.replace(" ","");
    //split
    std::vector<String> listMode;
    smode.split("|",listMode);
    //mode out:
    int mode=Object::DISABLE_PARENT;
    //for each mode
    for(auto& astr:listMode)
    {
        if(astr=="ALL")
            return Object::ENABLE_ALL;

        else if(astr=="POSITION")
            mode|=Object::ENABLE_POSITION;
        else if(astr=="ROTATION")
            mode|=Object::ENABLE_ROTATION;
        else if(astr=="SCALE")
            mode|=Object::ENABLE_SCALE;
        
        if(mode==Object::ENABLE_ALL)
            return Object::ENABLE_ALL;
    }
    return mode;
}
//serialize/deserialize
void Object::serialize(Table& table)
{
    Table& robj=table.createTable(getName());
    robj.set("position",getPosition());
    robj.set("rotation",getRotation().valueDegrees());
    robj.set("scale",getScale());
    robj.set("z",getZ());
    
    if(parent)
        robj.set("parentMode",parentToString(parentMode));
    //components
    Table& rcomponents=robj.createTable("Components");
    for(auto component:components)
    {
        Table& rcomponent=rcomponents.createTable(component.second->getComponentName());
        component.second->serialize(rcomponent);
    }
    //childs
    Table& rchilds=robj.createTable("Childs");
    for(auto child:childs)
    {
        child->serialize(rchilds);
    }

}
void Object::deserialize(const Table& table)
{
    //////////////////////////////////////////////////
    const auto name=table.getName();
    setName(name==""?getName():name);
    setPosition(table.getVector2D("position",getPosition()));
    setRotation(Angle( Degree( table.getFloat("rotation",   getRotation().valueDegrees() ) ) ) );
    setScale(table.getVector2D("scale",      getScale()));
    setZ(table.getFloat("z",getZ()));

    if(table.existsAsType("parentMode",Table::STRING))
    {
        const String& smode=table.getString("parentMode");
        auto mode=stringToParent(smode);
        setParentMode(mode);
    }
    //////////////////////////////////////////////////
    //component
    const Table& rcomponents=table.getConstTable("Components");
    for(auto rcomponent:rcomponents)
    {
        DEBUG_ASSERT(rcomponent.second->asType(Table::TABLE));
        auto cmp=component(rcomponent.first.string());
        cmp->setEntity(this);
        if(getScene()) cmp->onSetScene(scene);
        cmp->deserialize(rcomponent.second->get<Table>());
    }
    //childs
    const Table& rchilds=table.getConstTable("Childs");
    for(auto child:rchilds)
    {
        auto obj=new Object();
        addChild(obj,true);
        obj->deserialize(child.second->get<Table>());
    }    
    //////////////////////////////////////////////////

}
