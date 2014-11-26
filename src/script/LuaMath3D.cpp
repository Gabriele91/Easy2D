#include <stdafx.h>
#include <Math3D.h>
#include <ELuaState.h>

using namespace Easy2D;

static int LuaNewVec2(lua_State* lVM)
{  
    int const nargs = lua_gettop (lVM);
    //push a new vec2
    if(!nargs)
    {
        luabridge::Stack <Vec2>::push(lVM,Vec2());
    }
    else
    if(lua_isuserdata(lVM,1) && nargs==1)
    {
        luabridge::Stack <Vec2>::push(lVM, 
            Vec2(luabridge::Stack <Vec2>::get(lVM, 1)));
    }
    else
    if(lua_isnumber(lVM,1) && lua_isnumber(lVM,2) && nargs==2)
    {
        luabridge::Stack <Vec2>::push(lVM,
            Vec2(luabridge::Stack <float>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2)));
    }
    else
    {
        luaL_argerror(lVM,nargs,"Vec2.new fail");
        return 0;
    }
    return 1;
}
static int LuaNewVec3(lua_State* lVM)
{  
    int const nargs = lua_gettop (lVM);
    //push a new vec3
    if(!nargs)
    {
        luabridge::Stack <Vec3>::push(lVM,Vec3());
    }
    else
    if(lua_isuserdata(lVM,1) && nargs==1)
    {
        luabridge::Stack <Vec3>::push(lVM, 
            Vec3(luabridge::Stack <Vec3>::get(lVM, 1)));
    }
    else
    if(lua_isuserdata(lVM,1) && lua_isnumber(lVM,2) && nargs==2)
    {
        luabridge::Stack <Vec3>::push(lVM,
            Vec3(luabridge::Stack <Vec2>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2)));
    }
    else
    if(lua_isnumber(lVM,1) && 
       lua_isnumber(lVM,2) && 
       lua_isnumber(lVM,3) && nargs==3)
    {
        luabridge::Stack <Vec3>::push(lVM,
            Vec3(luabridge::Stack <float>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2),
                 luabridge::Stack <float>::get(lVM, 3)));
    }
    else
    {
        luaL_argerror(lVM,nargs,"Vec3.new fail");
        return 0;
    }
    return 1;
}
static int LuaNewVec4(lua_State* lVM)
{  
    int const nargs = lua_gettop (lVM);
    //push a new vec4
    if(!nargs)
    {
        luabridge::Stack <Vec4>::push(lVM,Vec4());
    }
    else
    if(lua_isuserdata(lVM,1) && nargs==1)
    {
        luabridge::Stack <Vec4>::push(lVM, 
            Vec4(luabridge::Stack <Vec4>::get(lVM, 1)));
    }
    else
    if(lua_isuserdata(lVM,1) && lua_isnumber(lVM,2) && nargs==2)
    {
        luabridge::Stack <Vec4>::push(lVM,
            Vec4(luabridge::Stack <Vec3>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2)));
    }
    else
    if(lua_isuserdata(lVM,1) && 
       lua_isnumber(lVM,2) && 
       lua_isnumber(lVM,3) && nargs==3)
    {
        luabridge::Stack <Vec4>::push(lVM,
            Vec4(luabridge::Stack <Vec2>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2),
                 luabridge::Stack <float>::get(lVM, 3)));
    }
    else
    if(lua_isnumber(lVM,1) && 
       lua_isnumber(lVM,2) && 
       lua_isnumber(lVM,3) && 
       lua_isnumber(lVM,4) && nargs==4)
    {
        luabridge::Stack <Vec4>::push(lVM,
            Vec4(luabridge::Stack <float>::get(lVM, 1),
                 luabridge::Stack <float>::get(lVM, 2),
                 luabridge::Stack <float>::get(lVM, 3),
                 luabridge::Stack <float>::get(lVM, 4)));
    }
    else
    {
        luaL_argerror(lVM,nargs,"Vec4.new fail");
        return 0;
    }
    return 1;
}
static int LuaNewMat4(lua_State* lVM)
{  
    int const nargs = lua_gettop (lVM);
    //push a new Mat4
    if(!nargs)
    {
        luabridge::Stack <Mat4>::push(lVM,Mat4());
    }
    else
    if(lua_isuserdata(lVM,1) && nargs==1)
    {
        luabridge::Stack <Mat4>::push(lVM, 
            Mat4(luabridge::Stack <Mat4>::get(lVM, 1)));
    }
    else
    {
        //is16Bool
        bool is16Bool=true && nargs==16;
        //all integer?
        for(uchar i=0;is16Bool && i!=16;++i) 
            is16Bool=is16Bool&&lua_isnumber(lVM,i);
        //put values
        if(is16Bool)
        {
            luabridge::Stack <Mat4>::push(lVM,
             Mat4(
             luaL_checknumber(lVM,1) ,luaL_checknumber(lVM,2), luaL_checknumber(lVM,3), luaL_checknumber(lVM,4),
             luaL_checknumber(lVM,5) ,luaL_checknumber(lVM,6), luaL_checknumber(lVM,7), luaL_checknumber(lVM,8),
             luaL_checknumber(lVM,9) ,luaL_checknumber(lVM,10),luaL_checknumber(lVM,11),luaL_checknumber(lVM,12),
             luaL_checknumber(lVM,13),luaL_checknumber(lVM,14),luaL_checknumber(lVM,15),luaL_checknumber(lVM,16)
             ));
        }
        else
        {
            luaL_argerror(lVM,nargs,"Mat4.new fail");
            return 0;
        }
    }
    return 1;
}

class LuaVec2 : public Vec2
{
    public:
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=(Vec2*)(this);//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["x"]=rsthis->x;
        t["y"]=rsthis->y;
        t.push(luaVM);
        //
        return 1;
    }
};
class LuaVec3 : public Vec3
{
    public:
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=(Vec3*)(this);//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["x"]=rsthis->x;
        t["y"]=rsthis->y;
        t["z"]=rsthis->z;
        t.push(luaVM);
        //
        return 1;
    }
};
class LuaVec4 : public Vec4
{
    public:
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=(Vec4*)(this);//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["x"]=rsthis->x;
        t["y"]=rsthis->y;
        t["z"]=rsthis->z;
        t["w"]=rsthis->w;
        t.push(luaVM);
        //
        return 1;
    }
};
class LuaAABox2 : public AABox2
{
    public:
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=(AABox2*)(this);;//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        t["center"]=rsthis->getCenter();
        t["size"]=rsthis->getSize();
        t.push(luaVM);
        //
        return 1;
    }
};
class LuaMat4 : public Mat4
{
    public:
    int __towatch (lua_State* luaVM)
    {
        //good cast
        auto rsthis=(Mat4*)(this);//1=self/this
        //call
        luabridge::LuaRef t = luabridge::newTable (luaVM);
        int i=0;
        for(auto v:rsthis->entries)
        {
           t[++i]=v;
        }
        t.push(luaVM);
        //
        return 1;
    }
};

void LuaState::addMath3DLib()
{
    /** VEC 2 CLASS */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Vec2>("Vec2")
        .addConstructor <void (*) (void)> ()
        .addStaticCFunction("new",&LuaNewVec2)
        .addData ("x",&Vec2::x)
        .addData ("y",&Vec2::y)
        .addFunction("dot",&Vec2::dot)
        .addFunction("abs",&Vec2::abs)
        .addFunction("getAbs",&Vec2::getAbs)
        .addFunction("normalize",&Vec2::normalize)
        .addFunction("getNormalize",&Vec2::getNormalize)
        .addFunction("length",&Vec2::length)
        .addFunction("squaredLength",&Vec2::squaredLength)
        .addFunction("distance",&Vec2::distance)
        .addFunction("distancePow2",&Vec2::distancePow2)
        .addFunction("cross",&Vec2::cross)
        .addFunction("axis",&Vec2::axis)
        .addFunction("direction",&Vec2::direction)
        .addFunction<Vec2(Vec2::*)(const Vec2&) const>("__add",&Vec2::operator+)
        .addFunction<Vec2(Vec2::*)(const Vec2&) const>("__sub",&Vec2::operator-)
        .addFunction<Vec2(Vec2::*)(void) const >("__unm",&Vec2::operator-)
        .addFunction<Vec2(Vec2::*)(const Vec2&) const>("__mul",&Vec2::operator*)
        .addFunction<Vec2(Vec2::*)(const Vec2&) const>("__div",&Vec2::operator/)
        .addCFunction("__towatch", (int (Vec2::*) (lua_State*))&LuaVec2::__towatch);
    /** VEC 3 CLASS */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Vec3>("Vec3")
        .addConstructor <void (*) (void)> ()
        .addStaticCFunction("new",&LuaNewVec3)
        .addData ("x",&Vec3::x)
        .addData ("y",&Vec3::y)
        .addData ("z",&Vec3::z)
        .addFunction("dot",&Vec3::dot)
        .addFunction("abs",&Vec3::abs)
        .addFunction("getAbs",&Vec3::getAbs)
        .addFunction("normalize",&Vec3::normalize)
        .addFunction("getNormalize",&Vec3::getNormalize)
        .addFunction("length",&Vec3::length)
        .addFunction("squaredLength",&Vec3::squaredLength)
        .addFunction("distance",&Vec3::distance)
        .addFunction("distancePow2",&Vec3::distancePow2)
        .addFunction("cross",&Vec3::cross)
        .addFunction<Vec3(Vec3::*)(const Vec3&) const>("__add",&Vec3::operator+)
        .addFunction<Vec3(Vec3::*)(const Vec3&) const>("__sub",&Vec3::operator-)
        .addFunction<Vec3(Vec3::*)(void) const >("__unm",&Vec3::operator-)
        .addFunction<Vec3(Vec3::*)(const Vec3&) const>("__mul",&Vec3::operator*)
        .addFunction<Vec3(Vec3::*)(const Vec3&) const>("__div",&Vec3::operator/)
        .addCFunction("__towatch", (int (Vec3::*) (lua_State*))&LuaVec3::__towatch);
;
    /** VEC 4 CLASS */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Vec4>("Vec4")
        .addConstructor <void (*) (void)> ()
        .addStaticCFunction("new",&LuaNewVec4)
        .addData ("x",&Vec4::x)
        .addData ("y",&Vec4::y)
        .addData ("z",&Vec4::z)
        .addData ("w",&Vec4::w)
        .addFunction("dot",&Vec4::dot)
        .addFunction("abs",&Vec4::abs)
        .addFunction("getAbs",&Vec4::getAbs)
        .addFunction("normalize",&Vec4::normalize)
        .addFunction("getNormalize",&Vec4::getNormalize)
        .addFunction("length",&Vec4::length)
        .addFunction("squaredLength",&Vec4::squaredLength)
        .addFunction("distance",&Vec4::distance)
        .addFunction("distancePow2",&Vec4::distancePow2)
        .addFunction<Vec4(Vec4::*)(const Vec4&) const>("__add",&Vec4::operator+)
        .addFunction<Vec4(Vec4::*)(const Vec4&) const>("__sub",&Vec4::operator-)
        .addFunction<Vec4(Vec4::*)(void) const >("__unm",&Vec4::operator-)
        .addFunction<Vec4(Vec4::*)(const Vec4&) const>("__mul",&Vec4::operator*)
        .addFunction<Vec4(Vec4::*)(const Vec4&) const>("__div",&Vec4::operator/)
        .addCFunction("__towatch", (int (Vec4::*) (lua_State*))&LuaVec4::__towatch);
    /** AABOX2 CLASS */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<AABox2>("AABox2")
        .addConstructor <void (*) (void)> ()
        .addFunction("setBox",&AABox2::setBox)
        .addFunction("setRegion",&AABox2::setRegion)
        .addFunction("getCenter",&AABox2::getCenter)
        .addFunction("getSize",&AABox2::getSize)

        .addFunction("setMax",&AABox2::setMax)
        .addFunction("setMin",&AABox2::setMin)
        //.addFunction("getMax",&AABox2::getMax)
        //.addFunction("getMin",&AABox2::getMin)

        .addFunction("addPoint",&AABox2::addPoint)

        .addFunction<bool (AABox2::*)(const Vec2&) const>("isIntersection",&AABox2::isIntersection)
        .addFunction<bool (AABox2::*)(const AABox2&) const>("isIntersection",&AABox2::isIntersection)
        .addFunction<AABox2 (AABox2::*)(const Mat4&) const>("applay",&AABox2::applay)
        .addCFunction("__towatch", (int (AABox2::*) (lua_State*))&LuaAABox2::__towatch);
    
    /** MAT4 CLASS */
    luabridge::getGlobalNamespace(luaVM)
        .beginClass<Mat4>("Mat4")
        .addConstructor <void (*) (void)> ()
        .addStaticCFunction("new",&LuaNewMat4)
        /* max 14 params
        .addConstructor <void (*) (float,float,float,float,
                                   float,float,float,float,
                                   float,float,float,float,
                                   float,float,float,float)> ()*/
        .addData ("m00",&Mat4::m00)
        .addData ("m10",&Mat4::m10)
        .addData ("m20",&Mat4::m20)
        .addData ("m30",&Mat4::m30)

        .addData ("m01",&Mat4::m01)
        .addData ("m11",&Mat4::m11)
        .addData ("m21",&Mat4::m21)
        .addData ("m31",&Mat4::m31)

        .addData ("m02",&Mat4::m02)
        .addData ("m12",&Mat4::m12)
        .addData ("m22",&Mat4::m22)
        .addData ("m32",&Mat4::m32)

        .addData ("m03",&Mat4::m03)
        .addData ("m13",&Mat4::m13)
        .addData ("m23",&Mat4::m23)
        .addData ("m33",&Mat4::m33)
        //MATRIX OP
        .addFunction("identity",&Mat4::identity)
        .addFunction("zero",&Mat4::zero)
        .addFunction("inverse",&Mat4::inverse)
        .addFunction("inverse2D",&Mat4::inverse2D)
        .addFunction("getInverse",&Mat4::getInverse)
        .addFunction("getTranspose",&Mat4::getTranspose)
        .addFunction< Mat4 (Mat4::*)(const Mat4 &) const >("mul",&Mat4::mul)
        .addFunction< Mat4 (Mat4::*)(const Mat4 &) const >("mul2D",&Mat4::mul2D)
        .addFunction< Vec4 (Mat4::*)(const Vec4 &) const >("mul",&Mat4::mul)
        .addFunction< Vec2 (Mat4::*)(const Vec2 &) const >("mul2D",&Mat4::mul2D)
        //PROSPETIVE
        .addFunction("setOrtho",&Mat4::setOrtho)
        .addFunction< void (Mat4::*)(float,float,float,float,float,float) >("setPerspective",&Mat4::setPerspective)
        .addFunction< void (Mat4::*)(float,float,float,float) >("setPerspective",&Mat4::setPerspective)
        //ROTATION
        .addFunction("setRotX",&Mat4::setRotX)
        .addFunction("setRotY",&Mat4::setRotY)
        .addFunction("setRotZ",&Mat4::setRotZ)        
        .addFunction("getRotX",&Mat4::getRotX)
        .addFunction("getRotY",&Mat4::getRotY)
        .addFunction("getRotZ",&Mat4::getRotZ)
        //TRANSLATION
        .addFunction<void (Mat4::*)(const Vec2&)>("setTranslation2D",&Mat4::setTranslation)
        .addFunction<void (Mat4::*)(const Vec3&)>("setTranslation3D",&Mat4::setTranslation)
        .addFunction("getTranslation2D",&Mat4::getTranslation2D)
        .addFunction("getTranslation3D",&Mat4::getTranslation3D)
        //SCALE
        .addFunction<void (Mat4::*)(const Vec2&)>("setScale2D",&Mat4::setScale)
        .addFunction<void (Mat4::*)(const Vec3&)>("setScale3D",&Mat4::setScale)
        .addFunction<void (Mat4::*)(const Vec2&)>("addScale2D",&Mat4::addScale)
        .addFunction<void (Mat4::*)(const Vec3&)>("addScale3D",&Mat4::addScale)
        .addFunction("getScale2D",&Mat4::getScale2D)
        .addFunction("getScale3D",&Mat4::getScale3D)
        .addCFunction("__towatch", (int (Mat4::*) (lua_State*))&LuaMat4::__towatch);
    
}
