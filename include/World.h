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
    virtual void DrawPoint(const b2Vec2& center, float32 size, const b2Color& color)
    {
        DrawCircle(center, size, color);
    };
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
class World : public AlignedAlloc<16>,
              public b2DestructionListener
{

    friend class Body;
    friend class GLDebugDraw;
    friend class ContactListener;  
    //world
    b2Body  *ground;
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
    UnorderedMap<uint, b2Joint*> jointHash;
    UnorderedMap<b2Joint*, uint> reverseJointHash;
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
    void  physicsDebugDraw(bool enable);
    ///raycast
    void raycast(Function<int(Body* body)>,const Vec2& start,const Vec2& end) const;
    void raycast(Function<int(Body* body,
                               Shape shape,
                               const Vec2 & 	point,
                               const Vec2 & 	normal,
                               float32 	fraction)>,
                 const Vec2& start,
                 const Vec2& end) const;
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
    void setPositionIterations( uint iterations ) 
    { 
        positionIterations = iterations; 
    }
    uint getPositionIterations() const 
    { 
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
    
    /// Weld joint
    uint createWeldJoint(const Object* objectA,
                         const Object* objectB,
                         const Vec2& localAnchorA = Vec2::ZERO,
                         const Vec2& localAnchorB = Vec2::ZERO,
                         Angle referenceAngle = Angle::radian(0.0f),
                         float frequency = 0.0f,
                         float dampingRatio = 0.0f,
                         bool collideConnected = false );
    
    void setWeldJointAngle(uint jointId,Angle angle);
    
    Angle getWeldJointAngle(uint jointId);
    
    void setWeldJointFrequency(uint jointId,float frequency);
    
    float getWeldJointFrequency(uint jointId);
    
    void setWeldJointDampingRatio(uint jointId,float dampingRatio);
    
    float getWeldJointDampingRatio(uint jointId);
    
    /// Mouse joint
    uint createMouseJoint(const Object* object,
                          const Vec2& target=Vec2::ZERO,
                          float force = 0.0f,
                          float frequency = 0.0f,
                          float dampingRatio = 0.0f,
                          bool collideConnected = false );
    
    void setMouseJointTarget(uint jointId,const Vec2& value);
    
    Vec2 getMouseJointTarget(uint jointId);
    
    void setMouseJointForce(uint jointId,float force);
    
    float getMouseJointForce(uint jointId);
    
    void setMouseJointFrequency(uint jointId,float frequency);
    
    float getMouseJointFrequency(uint jointId);
    
    void setMouseJointDampingRatio(uint jointId,float dampingRatio);
    
    float getMouseJointDampingRatio(uint jointId);
    
    //raw joints
    uint createJoint( const b2JointDef& def );
    void deleteJoint( uint id );
    
};


};

#endif
