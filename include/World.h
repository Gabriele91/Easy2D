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
class World;
class GLDebugDraw;
class ContactListener;
/////////////////////
class GLDebugDraw : public b2Draw
{

    World* world;

    public:

    GLDebugDraw(World* world):world(world){}

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
    World* world;

    public:

    ContactListener(World* world):world(world){}

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};
/////////////////////
class World : public b2DestructionListener
{

    friend class Body;
    friend class GLDebugDraw;
    friend class ContactListener;  
    //world
    b2World *world;
    b2BlockAllocator blockAllocator;
    GLDebugDraw      glDebugDraw;
    ContactListener  cListener;
    uint velocityIterations;
    uint positionIterations;
    //draw debug status
    bool debugFlag;
    //listener
    virtual void SayGoodbye( b2Joint* pJoint );
    virtual void SayGoodbye( b2Fixture* pFixture ) {}
    //join
    uint autoIdJoin;
    DUNORDERED_MAP<uint, b2Joint*> jointHash;
    DUNORDERED_MAP<b2Joint*, uint> reverseJointHash;
    uint createJoint( const b2JointDef& def );
    void deleteJoint(uint id);
    //join utility
    b2Joint* findB2Joint(uint id);
    uint findJoint(b2Joint* join);
    //meters unit
    float metersUnit;
    float metersInPixel;

    protected:

    void  physicsDraw(Camera* camera);
    void  physics(float dt);

    public:

    World(const Vec2& gravity=Vec2::ZERO);
    virtual ~World();
    void  setGravity(const Vec2& gravity);
    Vec2  getGravity();
    void  debugDraw(bool enable);
    ///scale
    void setMetersInPixel( float pixel );
    float getMetersInPixel() const
    {
        return metersInPixel;
    }
    float getMetersUint() const
    {
        return metersUnit;
    }
    //reset
    void resetWorld();
    ///serialize/deserialize
    void serialize(Table& table);
    void deserialize(const Table& table);
    ///Box2d item
    b2World*  getWorld()
    {
        return world;
    }
    const b2World*  getWorld() const
    {
        return world;
    }
    ///iterations
    void setVelocityIterations( uint iterations ) 
    {
        velocityIterations = iterations;
    }
    uint getVelocityIterations() const
    { 
        return velocityIterations;
    }
    void setPositionIterations( uint iterations ) { 
        positionIterations = iterations; 
    }
    uint getPositionIterations() const { 
        return positionIterations; 
    }


    /// Joints
    /// Distance joint.
    uint createDistanceJoint(const Object* objectA,
                             const Object* objectB,
                             const Vec2& localAnchorA = Vec2::ZERO, 
                             const Vec2& localAnchorB = Vec2::ZERO, 
                             float length = -1.0f,
                             float frequency = 0.0f,
                             float dampingRatio = 0.0f,
                             bool collideConnected = false );

    void setDistanceJointLength(uint jointId,float length);

    float getDistanceJointLength(uint jointId);

    void setDistanceJointFrequency(uint jointId,float frequency);

    float getDistanceJointFrequency(uint jointId);

    void setDistanceJointDampingRatio(uint jointId,float dampingRatio);

    float getDistanceJointDampingRatio(uint jointId);
    
    /// Revolute joint.
    uint createRevoluteJoint(const Object* objectA, 
                             const Object* objectB,
                             const Vec2& localAnchorA = Vec2::ZERO, 
                             const Vec2& localAnchorB = Vec2::ZERO,
                             bool collideConnected = false );

    void setRevoluteJointLimit(uint jointId,
                               bool enableLimit,
                               float lowerAngle,
                               float upperAngle );

    bool getRevoluteJointLimit(uint jointId,
                               bool& enableLimit,
                               float& lowerAngle,
                               float& upperAngle );

    void setRevoluteJointMotor(uint jointId,
                               bool enableMotor,
                               float motorSpeed = Math::PI,
                               float maxMotorTorque = 0.0f );

    bool getRevoluteJointMotor(uint jointId,
                               bool& enableMotor,
                               float& motorSpeed,
                               float& maxMotorTorque );

    float getRevoluteJointAngle(uint jointId);
    float getRevoluteJointSpeed(uint jointId);
};


};

#endif