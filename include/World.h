#ifndef WORLD_H
#define WORLD_H

#include <Config.h>
#include <Math3D.h>
#include <Box2D.hpp>
#include <Object.h>
#include <Camera.h>

namespace Easy2D
{
/////////////////////
class Body;
/////////////////////
class GLDebugDraw : public b2Draw
{
    public:
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    virtual void DrawTransform(const b2Transform& xf);
};
/////////////////////
class ContactListener : public b2ContactListener
{
    public:
        ContactListener(){}

        virtual void BeginContact(b2Contact* contact);
        virtual void EndContact(b2Contact* contact);
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};
/////////////////////
class World
{

    friend class Body;
    b2World *world;
    b2BlockAllocator blockAllocator;
    GLDebugDraw      debugDraw;

    protected:

    void  physicsDraw(Camera* camera);

    public:

    World(const Vec2& gravity=Vec2::ZERO);
    virtual ~World();
    void  setGravity(const Vec2& gravity);
    void  physics(float dt, uint velocityIterations=8, uint positionIterations=3);
    void  enableDebugDraw();
};


};

#endif