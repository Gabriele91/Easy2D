#include <stdafx.h>
#include <Body.h>
#include <ELuaState.h>
#include <LuaBridge/LuaBridge.h>
////////////////////////
using namespace Easy2D;
////////////////////////

#define GeS(X) X,X

class LuaBody : public Body
{
    template<class T>
    bool luaTableToCPPVector(lua_State* luaVM,
                             int i,
                             std::vector<T>& out)
    {
        luabridge::LuaRef tableRef(luaVM,i);
        if(tableRef.isTable())
        {
            //array len
            size_t len=tableRef.length();
            //to c++ object
            std::vector<Vec2> vspolygon(len);
            for (size_t i=0; i!=len; ++i)
            {
                vspolygon[i]=tableRef[i].cast<Vec2>();
            }
            return true;
        }
        return false;
    }
    
public:
    //body type
    int setType(lua_State* luaVM){
        //nargs
        int const nargs = lua_gettop (luaVM);
        //1 is self
        auto bthis=(Body*)this;
        //arg
        if(lua_isnumber(luaVM,2) && nargs==2)
        {
            //set type
            bthis->setType((Body::Type)luabridge::Stack <int>::get(luaVM,2));
        }
        else
        {
            luaL_argerror(luaVM,nargs,"Body::setType fail");
        }
        //set type
        return 0;
    }
    
    //get body type
    int getType(lua_State* luaVM){
        //nargs
        int const nargs = lua_gettop (luaVM);
        //1 is self
        auto bthis=(Body*)this;
        //only this
        if(nargs!=1)
        {
            luaL_argerror(luaVM,nargs,"Body::getType fail");
        }
        //push return
        luabridge::Stack<int>::push(luaVM,(int)bthis->getType());
        //set type
        return 1;
    }
    
    
    //callbacks
    int setBeginListener(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2){ luaL_argerror(luaVM,nargs,"Body::setBeginListener fail"); return 0; }
        //get callback
        luabridge::LuaRef function=luabridge::Stack<luabridge::LuaRef>::get(luaVM,2);
        if(!function.isFunction()){ luaL_argerror(luaVM,nargs,"Body::setBeginListener fail"); return 0; }
        //add listener
        Body::setBeginListener([function](Object* tocollide,const Info& info)
                               {
                                   //call lua
                                   function(tocollide,info);
                               });
        return 0;
    }
    int setEndListener(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2){ luaL_argerror(luaVM,nargs,"Body::setEndListener fail"); return 0; }
        //get callback
        luabridge::LuaRef function=luabridge::Stack<luabridge::LuaRef>::get(luaVM,2);
        if(!function.isFunction()){ luaL_argerror(luaVM,nargs,"Body::setEndListener fail"); return 0; }
        //add listener
        Body::setEndListener([function](Object* tocollide,const Info& info)
                             {
                                 //call lua
                                 function(tocollide,info);
                             });
        return 0;
    }
    int setPreSolverListener(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2){ luaL_argerror(luaVM,nargs,"Body::setPreSolverListener fail"); return 0; }
        //get callback
        luabridge::LuaRef function=luabridge::Stack<luabridge::LuaRef>::get(luaVM,2);
        if(!function.isFunction()){ luaL_argerror(luaVM,nargs,"Body::setPreSolverListener fail"); return 0; }
        //add listener
        Body::setPreSolverListener([function](Object* tocollide,const Info& info,const Manifold& oldmf)
                                   {
                                       //call lua
                                       function(tocollide,info,oldmf);
                                   });
        return 0;
    }
    int setPostSolverListener(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2){ luaL_argerror(luaVM,nargs,"Body::setPostSolverListener fail"); return 0; }
        //get callback
        luabridge::LuaRef function=luabridge::Stack<luabridge::LuaRef>::get(luaVM,2);
        if(!function.isFunction()){ luaL_argerror(luaVM,nargs,"Body::setPostSolverListener fail"); return 0; }
        //add listener
        Body::setPostSolverListener([function](Object* tocollide,const Info& info,const Impulse& impulse)
                                    {
                                        //call lua
                                        function(tocollide,info,impulse);
                                    });
        return 0;
    }
    
    //SHAPE CREATORS
    //createCircleShape
    int createCircleShape(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //Shape id
        Shape shape;
        //cases
        switch (nargs) {
            case 2:
                shape=this->Easy2D::Body::createCircleShape(luabridge::Stack<float>::get(luaVM,2));
                break;
            case 3:
                shape=this->Easy2D::Body::createCircleShape(luabridge::Stack<float>::get(luaVM,2),
                                                            luabridge::Stack<Vec2>::get(luaVM,3));
                break;
                
            default:
                luaL_argerror(luaVM,nargs,"Body::createCircleShape fail");
                return 0;
                break;
        }
        //return
        luabridge::Stack<unsigned int>::push(luaVM, shape);
        return 1;
    }
    //createBoxShape
    int createBoxShape(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //Shape id
        Shape shape;
        //cases
        switch (nargs) {
            case 2:
                shape=this->Easy2D::Body::createBoxShape(luabridge::Stack<Vec2>::get(luaVM,2));
                break;
            case 3:
                shape=this->Easy2D::Body::createBoxShape(luabridge::Stack<Vec2>::get(luaVM,2),
                                                         luabridge::Stack<Vec2>::get(luaVM,3));
                break;
            case 4:
                shape=this->Easy2D::Body::createBoxShape(luabridge::Stack<Vec2>::get(luaVM,2),
                                                         luabridge::Stack<Vec2>::get(luaVM,3),
                                                         luabridge::Stack<float>::get(luaVM,4));
                break;
            default:
                luaL_argerror(luaVM,nargs,"Body::createBoxShape fail");
                return 0;
                break;
        }
        //return
        luabridge::Stack<unsigned int>::push(luaVM, shape);
        return 1;
    }
    //createPolygonShape
    int createPolygonShape(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //Shape id
        Shape shape;
        //cases
        if(nargs==2)
        {
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,2,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //create shape
                    shape=this->Easy2D::Body::createPolygonShape(vspolygon);
                    //return
                    luabridge::Stack<unsigned int>::push(luaVM, shape);
                    return 1;
                }
            }
        }
        
        luaL_argerror(luaVM,nargs,"Body::createPolygonShape fail");
        return 0;
    }
    //createChainPoints
    int createChainPoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //Shape id
        Shape shape;
        //cases
        if(nargs==2)
        {
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,2,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //create shape
                    shape=this->Easy2D::Body::createChainShape(vspolygon);
                    //return
                    luabridge::Stack<unsigned int>::push(luaVM, shape);
                    return 1;
                }
            }
        }
        else if(nargs==6)
        {
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,2,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //create shape
                    shape=this->Easy2D::Body::createChainShape(vspolygon,
                                                               luabridge::Stack<bool>::get(luaVM,3),
                                                               luabridge::Stack<Vec2>::get(luaVM,4),
                                                               luabridge::Stack<bool>::get(luaVM,5),
                                                               luabridge::Stack<Vec2>::get(luaVM,6));
                    //return
                    luabridge::Stack<unsigned int>::push(luaVM, shape);
                    return 1;
                }
            }
        }
        
        luaL_argerror(luaVM,nargs,"Body::changeChainPoints fail");
        return 0;
    }
    //createEdgeShape
    int createEdgeShape(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //Shape id
        Shape shape;
        //cases
        switch (nargs) {
            case 3:
                shape=this->Easy2D::Body::createEdgeShape(luabridge::Stack<Vec2>::get(luaVM,2),
                                                          luabridge::Stack<Vec2>::get(luaVM,3));
                break;
            case 7:
                shape=this->Easy2D::Body::createEdgeShape(luabridge::Stack<Vec2>::get(luaVM,2),
                                                          luabridge::Stack<Vec2>::get(luaVM,3),
                                                          luabridge::Stack<bool>::get(luaVM,4),
                                                          luabridge::Stack<Vec2>::get(luaVM,5),
                                                          luabridge::Stack<bool>::get(luaVM,6),
                                                          luabridge::Stack<Vec2>::get(luaVM,7));
                break;
            default:
                luaL_argerror(luaVM,nargs,"Body::createEdgeShape fail");
                return 0;
                break;
        }
        //return
        luabridge::Stack<unsigned int>::push(luaVM, shape);
        return 1;
    }
    
    //GETTERS
    //get list of shape
    int listOfShapes(lua_State* luaVM)
    {
        //
        luabridge::LuaRef table=luabridge::newTable(luaVM);
        //n.b. lua table start to 1
        int i=0;
        //add elements
        for (Shape shape:*this)
        {
            table[++i]=shape;
        }
        //return
        luabridge::Stack<luabridge::LuaRef>::push(luaVM, table);
        return 1;
    }
    //get vertexs of polygon
    int getPolygonPoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getPolygonPoints fail");
            return 0;
        }
        //get list
        std::vector<Vec2> vlist;
        Body::getPolygonPoints(luabridge::Stack<int>::get(luaVM,2), vlist);
        //ret table
        luabridge::LuaRef table=luabridge::newTable(luaVM);
        //n.b. lua table start to 1
        int i=0;
        //add elements
        for (const Vec2& point:vlist)
        {
            table[++i]=point;
        }
        //return table
        luabridge::Stack<luabridge::LuaRef>::push(luaVM, table);
        return 1;
    }
    //get vertexs of chain
    int getChainPoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getChainPoints fail");
            return 0;
        }
        //get list
        std::vector<Vec2> vlist;
        Body::changeChainPoints(luabridge::Stack<int>::get(luaVM,2), vlist);
        //ret table
        luabridge::LuaRef table=luabridge::newTable(luaVM);
        //n.b. lua table start to 1
        int i=0;
        //add elements
        for (const Vec2& point:vlist)
        {
            table[++i]=point;
        }
        //return table
        luabridge::Stack<luabridge::LuaRef>::push(luaVM, table);
        return 1;
    }
    int getChainStartPoint(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getChainStartPoint fail");
            return 0;
        }
        //get list
        Vec2 value;
        Body::getChainStartPoint(luabridge::Stack<int>::get(luaVM,2), value);
        //return point
        luabridge::Stack<Vec2>::push(luaVM, value);
        return 1;
    }
    int getChainEndPoint(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getChainEndPoint fail");
            return 0;
        }
        //get list
        Vec2 value;
        Body::getChainEndPoint(luabridge::Stack<int>::get(luaVM,2), value);
        //return point
        luabridge::Stack<Vec2>::push(luaVM, value);
        return 1;
    }
    //get edge
    int getEdgePoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getEdgePoints fail");
            return 0;
        }
        //get list
        Vec2 value1;
        Vec2 value2;
        Body::getEdgePoints(luabridge::Stack<int>::get(luaVM,2), value1,value2);
        //return points
        luabridge::Stack<Vec2>::push(luaVM, value1);
        luabridge::Stack<Vec2>::push(luaVM, value2);
        return 2;
    }
    int getEdgeStartPoint(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getEdgeStartPoint fail");
            return 0;
        }
        //get list
        Vec2 value;
        Body::getEdgeStartPoint(luabridge::Stack<int>::get(luaVM,2), value);
        //return point
        luabridge::Stack<Vec2>::push(luaVM, value);
        return 1;
    }
    int getEdgeEndPoint(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        if(nargs!=2)
        {
            luaL_argerror(luaVM,nargs,"Body::getEdgeEndPoint fail");
            return 0;
        }
        //get list
        Vec2 value;
        Body::getEdgeEndPoint(luabridge::Stack<int>::get(luaVM,2), value);
        //return point
        luabridge::Stack<Vec2>::push(luaVM, value);
        return 1;
    }
    
    
    //CHANGERS
    //createPolygonShape
    int changePolygonPoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //cases
        if(nargs==3)
        {
            Shape shape=luabridge::Stack<int>::get(luaVM,2);
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,3,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //change shape
                    Body::changePolygonPoints(shape,vspolygon);
                    //return
                    return 0;
                }
            }
        }
        luaL_argerror(luaVM,nargs,"Body::changePolygonPoints fail");
        return 0;
    }
    //changeChainPoints
    int changeChainPoints(lua_State* luaVM)
    {
        //nargs
        int const nargs = lua_gettop (luaVM);
        //cases
        if(nargs==3)
        {
            Shape shape=luabridge::Stack<int>::get(luaVM,2);
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,3,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //change shape
                    Easy2D::Body::changeChainPoints(shape,vspolygon);
                    //return
                    return 0;
                }
            }
        }
        else if(nargs==7)
        {
            Shape shape=luabridge::Stack<int>::get(luaVM,2);
            //get c++ array
            std::vector<Vec2> vspolygon;
            if(luaTableToCPPVector(luaVM,3,vspolygon))
            {
                //triangle....
                if(vspolygon.size()>=3)
                {
                    //create shape
                    Easy2D::Body::changeChainPoints(shape,
                                                    vspolygon,
                                                    luabridge::Stack<bool>::get(luaVM,4),
                                                    luabridge::Stack<Vec2>::get(luaVM,5),
                                                    luabridge::Stack<bool>::get(luaVM,6),
                                                    luabridge::Stack<Vec2>::get(luaVM,7));
                    //return
                    return 0;
                }
            }
        }
        
        luaL_argerror(luaVM,nargs,"Body::changeChainPoints fail");
        return 0;
    }
    
    
    //towatch
    int __towatch (lua_State* luaVM)
    {
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["type"]="Body";
        t["shapes"]=countShape();
        t.push(luaVM);
        //one push
        return 1;
    }
};



template <class T,class R,size_t offset>
struct ArrayHelper 
{
    
    template <size_t index>
    static R get (T const* obj)
    {
        const R* array=(R*)(obj+offset);
        return array [index];
    }
    
    template <size_t index>
    static void set (T* obj, R value)
    {
        R* array=(R*)(obj+offset);
        array[index] = value;
    }
    
};
#define ArrayValue(Name,Obj,R,Att,i) # Name, \
                                     &ArrayHelper<Obj,R,offsetof(Obj,Att)>::get<i>, \
                                     &ArrayHelper<Obj,R,offsetof(Obj,Att)>::set<i>
typedef int(*getInt)();
#define GET_CONT(C) ((getInt)([]()->int { return C; }))


void LuaState::addBodyLib()
{
    /** Impulse */
    luabridge::getGlobalNamespace(luaVM)
    .beginClass<Body::Impulse>("Impulse")
    .addData("npoints",&Body::Impulse::npoints)
    .addProperty(ArrayValue( normalImpulse0, Body::Impulse, float, normalImpulse, 0))
    .addProperty(ArrayValue( normalImpulse1, Body::Impulse, float, normalImpulse, 1))
    .addProperty(ArrayValue( tangentImpulse0, Body::Impulse, float, tangentImpulse, 0))
    .addProperty(ArrayValue( tangentImpulse1, Body::Impulse, float, tangentImpulse, 1));
    
    /** Impulse */
    luabridge::getGlobalNamespace(luaVM)
    .beginClass<Body::Manifold>("Manifold")
    .addData("npoints",&Body::Manifold::npoints)
    .addData("normal", &Body::Manifold::normal)
    .addProperty(ArrayValue( points0, Body::Manifold, Vec2, points, 0))
    .addProperty(ArrayValue( points1, Body::Manifold, Vec2, points, 1))
    .addProperty(ArrayValue( normalImpulse0, Body::Manifold, float, normalImpulse, 0))
    .addProperty(ArrayValue( normalImpulse1, Body::Manifold, float, normalImpulse, 1))
    .addProperty(ArrayValue( tangentImpulse0, Body::Manifold, float, tangentImpulse, 0))
    .addProperty(ArrayValue( tangentImpulse1, Body::Manifold, float, tangentImpulse, 1));
    
    /** Info */
    luabridge::getGlobalNamespace(luaVM)
    .beginClass<Body::Info>("Info")
    .addData("shapeA",&Body::Info::shapeA)
    .addData("shapeB",&Body::Info::shapeB)
    //b2Contact* contact;
    .addData("manifold",&Body::Info::manifold)
    .addData("orderCase",&Body::Info::orderCase),
    
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace ("BodyType")
    .addProperty<int,int>("DINAMIC", GET_CONT(Body::DINAMIC))
    .addProperty<int,int>("KINEMATIC", GET_CONT(Body::KINEMATIC))
    .addProperty<int,int>("STATIC", GET_CONT(Body::STATIC))
    .endNamespace ();
    
    luabridge::getGlobalNamespace(luaVM)
    .beginNamespace ("BodyGeometryType")
    .addProperty<int,int>("CIRCLE", GET_CONT(Body::CIRCLE))
    .addProperty<int,int>("CHAIN", GET_CONT(Body::CHAIN))
    .addProperty<int,int>("POLYGON", GET_CONT(Body::POLYGON))
    .addProperty<int,int>("EDGE", GET_CONT(Body::EDGE))
    .addProperty<int,int>("NEXTGT", GET_CONT(Body::NEXTGT))
    .endNamespace ();
    
    /** Body */
    luabridge::getGlobalNamespace(luaVM)
	.deriveClass<Body, Component>("Body")
	.addConstructor <void(*) (void), RefCountedPtr <Body> >()
    .addCFunction("setType",(int (Body::*) (lua_State*))(&LuaBody::setType))
    .addCFunction("getType",(int (Body::*) (lua_State*))(&LuaBody::getType))
    //callbacks
    .addCFunction("setBeginListener",(int (Body::*) (lua_State*))(&LuaBody::setBeginListener))
    .addCFunction("setEndListener",(int (Body::*) (lua_State*))(&LuaBody::setEndListener))
    .addCFunction("setPreSolverListener",(int (Body::*) (lua_State*))(&LuaBody::setPreSolverListener))
    .addCFunction("setPostSolverListener",(int (Body::*) (lua_State*))(&LuaBody::setPostSolverListener))
    
    .addFunction("setActive", &Body::setActive)
    .addFunction("setAwake", &Body::setAwake)
    .addFunction("setBullet", &Body::setBullet)
    .addFunction("getActive", &Body::getActive)
    .addFunction("getAwake", &Body::getAwake)
    .addFunction("getBullet", &Body::getBullet)

    .addFunction("setEnableScale", &Body::setEnableScale)
    .addFunction("getEnableScale", &Body::getEnableScale)
    .addFunction("getMass", &Body::getMass)
    
    .addFunction("getWorldCenter", &Body::getWorldCenter)
    .addFunction("getWorldPoint", &Body::getWorldPoint)
    
    .addFunction("getLocalVector", &Body::getLocalVector)
    .addFunction("getLocalPoint", &Body::getLocalPoint)
    .addFunction("getLocalCenter", &Body::getLocalCenter)
    
    .addFunction("setLinearVelocity", &Body::setLinearVelocity)
    .addFunction("getLinearVelocity", &Body::getLinearVelocity)
    .addFunction("getLinearVelocityFromWorldPoint", &Body::getLinearVelocityFromWorldPoint)
    .addFunction("getLinearVelocityFromLocalPoint", &Body::getLinearVelocityFromLocalPoint)
    
    .addFunction("setLinearDamping", &Body::setLinearDamping)
    .addFunction("getLinearDamping", &Body::getLinearDamping)
    .addFunction("setAngularVelocity", &Body::setAngularVelocity)
    .addFunction("getAngularVelocity", &Body::getAngularVelocity)
    .addFunction("setAngularDamping", &Body::setAngularDamping)
    .addFunction("getAngularDamping", &Body::getAngularDamping)
    .addFunction("setFixedAngle", &Body::setFixedAngle)
    .addFunction("getFixedAngle", &Body::getFixedAngle)
    .addFunction("setGravityScale", &Body::setGravityScale)
    .addFunction("getGravityScale", &Body::getGravityScale)
    .addFunction("setSleepingAllowed", &Body::setSleepingAllowed)
    .addFunction("getSleepingAllowed", &Body::getSleepingAllowed)
    
    .addFunction("applyForce", &Body::applyForce)
    .addFunction("applyForceToCenter", &Body::applyForceToCenter)
    
    .addFunction("applyTorque", &Body::applyTorque)
    
    .addFunction("applyLinearImpulse", &Body::applyLinearImpulse)
    .addFunction("applyAngularImpulse", &Body::applyAngularImpulse)
    //SHAPE
    .addCFunction("createCircleShape",(int (Body::*) (lua_State*))&LuaBody::createCircleShape)
    .addCFunction("createBoxShape",(int (Body::*) (lua_State*))&LuaBody::createBoxShape)
    .addCFunction("createPolygonShape",(int (Body::*) (lua_State*))&LuaBody::createPolygonShape)
    .addCFunction("changeChainPoints",(int (Body::*) (lua_State*))&LuaBody::changeChainPoints)
    .addCFunction("createEdgeShape",(int (Body::*) (lua_State*))&LuaBody::createEdgeShape)
    
    .addFunction("setShapeDensity", &Body::setShapeDensity)
    .addFunction("getShapeDensity", &Body::getShapeDensity)
    
    .addFunction("setShapeFriction", &Body::setShapeFriction)
    .addFunction("getShapeFriction", &Body::getShapeFriction)
    
    .addFunction("setShapeRestitution", &Body::setShapeRestitution)
    .addFunction("getShapeRestitution", &Body::getShapeRestitution)
    
    .addFunction("setShapeIsSensor", &Body::setShapeIsSensor)
    .addFunction("getShapeIsSensor", &Body::getShapeIsSensor)
    
    .addFunction("countShape", &Body::countShape)
    .addFunction("getShapeType", &Body::getShapeType)
    .addCFunction("listOfShapes", (int (Body::*) (lua_State*))&LuaBody::listOfShapes)
    //circle
    .addFunction("getCirclePosition", &Body::getCirclePosition)
    .addFunction("getCircleRadius", &Body::getCircleRadius)
    //polygon
    .addCFunction("getPolygonPoints", (int (Body::*) (lua_State*))&LuaBody::getPolygonPoints)
    //chain
    .addCFunction("getChainPoints", (int (Body::*) (lua_State*))&LuaBody::getChainPoints)
    .addCFunction("getChainStartPoint", (int (Body::*) (lua_State*))&LuaBody::getChainStartPoint)
    .addCFunction("getChainEndPoint", (int (Body::*) (lua_State*))&LuaBody::getChainEndPoint)
    //edge
    .addCFunction("getEdgePoints", (int (Body::*) (lua_State*))&LuaBody::getEdgePoints)
    .addCFunction("getEdgeStartPoint", (int (Body::*) (lua_State*))&LuaBody::getEdgeStartPoint)
    .addCFunction("getEdgeStartPoint", (int (Body::*) (lua_State*))&LuaBody::getEdgeStartPoint)
    
    //CHANGE SHAPE GEOMETRY
    //circle
    .addFunction("changeCirclePosition", &Body::changeCirclePosition)
    .addFunction("changeCircleRadius", &Body::changeCircleRadius)
    //polygon
    .addCFunction("changePolygonPoints", (int (Body::*) (lua_State*))&LuaBody::changePolygonPoints)
    //chain
    .addCFunction("changeChainPoints", (int (Body::*) (lua_State*))&LuaBody::changePolygonPoints)
    
    //delete a shape
    .addFunction("deleteShape", &Body::deleteShape)
    
    //show type
    .addCFunction("__towatch",  (int (Body::*) (lua_State*))&LuaBody::__towatch);
;
    
}







