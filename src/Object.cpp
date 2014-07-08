#include <stdafx.h>
#include <Object.h>
#include <Body.h>
#include <Scene.h>
#include <Application.h>
/* Easy2D */
using namespace Easy2D;
//////////////////////
Object::Object()
    :data(nullptr)
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
    :data(nullptr)
    ,scene(nullptr)
    ,changeValue(false)
    ,del(false)
    ,parent(nullptr)
    ,parentMode(DISABLE_PARENT) 
    ,name(argname)
{
}
Object::~Object()
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
    //destroy components
    for(auto cmp:components)
        delete cmp.second;

}

void Object::setScale(const Vector2D &scale,bool global)
{
    if(!global||!parent)
        transform.scale=scale;
    else
    {
        transform.scale=scale/parent->getScale(true);
    }
    change();
}
void Object::setPosition(const Vector2D &position,bool global)
{
    if(!global||!parent)
        transform.position=position;
    else
        transform.position=position-parent->getPosition(true);

    change();
}
void Object::setRotation(float alpha,bool global)
{
    if(!global||!parent)
        transform.alpha=alpha;
    else
        transform.alpha=alpha-parent->getRotation(true);
    change();
}
void Object::setMove(const Vector2D &velocity)
{
    //
    float lcosf=std::cos(Math::torad(transform.alpha));
    float lsinf=std::sin(Math::torad(transform.alpha));
    //like openGL
    transform.position.x+= velocity.x*lcosf+velocity.y*lsinf;
    transform.position.y+=-velocity.x*lsinf+velocity.y*lcosf;
    //
    change();
}
void Object::setTranslation(const Vector2D &translation)
{
    transform.position+=translation;
    change();
}
void Object::setTurn(float alpha)
{
    transform.alpha+=alpha;
    change();
}
void Object::setParentMode(Object::ParentMode type)
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
    if(!global||!parent)
        return transform.scale;
    else
        return getGlobalMatrix().getScale2D();
}
Vector2D Object::getPosition(bool global)
{
    if(!global||!parent)
        return transform.position;
    else
        return getGlobalMatrix().getTranslation2D();
}
float Object::getRotation(bool global)
{
    if(!global||!parent)
        return transform.alpha;
    else
        return Math::todeg(getGlobalMatrix().getRotZ());
}
Object::ParentMode Object::getParentMode() const
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
void Object::change()
{
    if(!changeValue)
    {
        for(auto obj : *this )
        {
            obj->change();
        }
        changeValue=true;
    }
}
void Object::addChild(Object *child,bool ptrdelete)
{
    addChild(child,ParentMode::ENABLE_ALL,ptrdelete);
}
void Object::addChild(Object *child,ParentMode type,bool ptrdelete)
{

    if(child->parent==this) return;
    if(child==this) return;
    if(child->parent) child->parent->erseChild(child);

    child->del=ptrdelete;
    child->parent=this;
    child->parentMode=type;
    this->childs.push_back(child);
    this->setScene(getScene());

    child->change();

}
void Object::erseChild(Object *child)
{
    if(child->parent==this)
    {
        childs.remove(child);
        child->parent=NULL;
        child->change();
    }
}
size_t Object::countChilds()
{
    return childs.size();
}
size_t Object::depthChilds()
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
    auto it=components.find(cmp->getComponentInfo());
    if(it==components.end())
    {   
        components[cmp->getComponentInfo()]=cmp;
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
//Matrix
const Matrix4x4& Object::getGlobalMatrix()
{

    if(changeValue==true)
    {
        //
        globalMat.identity();
        //
        if(parent)
        {
            Matrix4x4 mtmp;
            mtmp=parent->getGlobalMatrix();
            Vector2D tmpPos(mtmp.entries[12],mtmp.entries[13]);

            switch(parentMode & (ENABLE_POSITION | ENABLE_ROTATION))
            {

            case ENABLE_ALL:
            case 3:			 //ENABLE_POSITION+ENABLE_ROTATION
                globalMat.setTransform2D(transform);
                mtmp.setRotZ(mtmp.getRotZ());
                mtmp.entries[12]=tmpPos.x;
                mtmp.entries[13]=tmpPos.y;
                globalMat=mtmp.mul2D(globalMat);
                break;

            case ENABLE_POSITION:
                globalMat.setRotZ(Math::torad(transform.alpha));
                globalMat.entries[12]=tmpPos.x+transform.position.x;
                globalMat.entries[13]=tmpPos.y+transform.position.y;
                break;

            case ENABLE_ROTATION:
                globalMat.setRotZ(mtmp.getRotZ()+Math::torad(transform.alpha));
                globalMat.entries[12]=transform.position.x;
                globalMat.entries[13]=transform.position.y;
                break;
            //VC9
            default:
                break;
            }

            if(ENABLE_SCALE & parentMode)
                globalMat.addScale(getGlobalParentScale());
            else
                globalMat.addScale(transform.scale);


        }
        else
            globalMat.setTransform2D(transform);
        //
        changeValue=false;
    }

    return globalMat;
}
Vector2D  Object::getGlobalParentScale()
{
    Object *p=NULL;
    Vector2D out=transform.scale;
    for(p=this->parent; p; p=p->parent )
    {
        if(ENABLE_SCALE & p->parentMode)
            out*=p->transform.scale;
        else
            out=p->transform.scale;
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

//utility methos
Screen* Object::getScreen()
{
    return Application::instance()->getScreen();
}
Audio* Object::getAudio()
{
    return Application::instance()->getAudio();
}
Input* Object::getInput()
{
    return Application::instance()->getInput();
}
Game* Object::getGame()
{
    return Application::instance()->getGame();
}
ResourcesGroup* Object::getResourcesGroup(const String& name)
{
    return Application::instance()->getResourcesGroup(name);
}

static String parentToString(Object::ParentMode mode)
{
    if(mode == Object::DISABLE_PARENT) return "DISABLE";
    if(mode == Object::ENABLE_ALL) return "ALL";

    String smode;
    if(mode & Object::ENABLE_POSITION) smode= "POSITION";
    if(mode & Object::ENABLE_ROTATION) smode=String(smode.size() ? "|" : "") + "ROTATION";
    if(mode & Object::ENABLE_SCALE)    smode=String(smode.size() ? "|" : "") + "SCALE";

    return smode;
}
static Object::ParentMode stringToParent(const String& argsmode)
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
    return (Object::ParentMode)mode;
}
//serialize/deserialize
void Object::serialize(Table& table)
{
    Table& robj=table.createTable(getName());
    robj.set("position",getPosition());
    robj.set("rotation",getRotation());
    robj.set("scale",getScale());
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
    setRotation(table.getFloat("rotation",getRotation()));
    setScale(table.getVector2D("scale",getScale()));
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