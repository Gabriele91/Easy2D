#include <stdafx.h>
#include <World.h>
#include <Body.h>
#include <Scene.h>
#include <Debug.h>
#include <RenderContext.h>

///////////////////////
using namespace Easy2D;
///////////////////////
#define PTM_RATIO *metersUnit
#define PIXEL_RATIO *metersInPixel
#define WORLD_PIXEL_RATIO *world->metersInPixel


#if 0
    #define FORCE_PTM_RATIO *metersUnit
    #define FORCE_PIXEL_RATIO *metersInPixel
#else //OLREADY ADDED IN BODY SCALE GRAVITY
    #define FORCE_PTM_RATIO
    #define FORCE_PIXEL_RATIO
#endif


////////////////////////////////////////
void ContactListener::BeginContact(b2Contact* contact)
{
    //Get body
    Body* bodyA=(Body*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Body* bodyB=(Body*)(contact->GetFixtureB()->GetBody()->GetUserData());
    //no callback?
    if( !bodyA->cbBegin && !bodyB->cbBegin ) return;
    //Get body
    Object* objectA=(bodyA->getObject());
    Object* objectB=(bodyB->getObject());
    //get index
    size_t indexA=(size_t)(contact->GetFixtureA()->GetUserData());
    size_t indexB=(size_t)(contact->GetFixtureB()->GetUserData());    
    //get info
    Body::Info info;
    info.contact=contact;
    info.manifold.setManifold(*contact->GetManifold());
    info.manifold.points[0]=info.manifold.points[0] WORLD_PIXEL_RATIO;
    info.manifold.points[1]=info.manifold.points[1] WORLD_PIXEL_RATIO;
    //callback
    if(bodyA->cbBegin)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        info.orderCase=1;
        bodyA->cbBegin(objectB,info);
    }
    if(bodyB->cbBegin)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
        info.orderCase=2;
        bodyB->cbBegin(objectA,info);
    }
}
void ContactListener::EndContact(b2Contact* contact)
{
    //Get body
    Body* bodyA=(Body*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Body* bodyB=(Body*)(contact->GetFixtureB()->GetBody()->GetUserData());
    //no callback?
    if( !bodyA->cbEnd && !bodyB->cbEnd ) return;
    //Get body
    Object* objectA=(bodyA->getObject());
    Object* objectB=(bodyB->getObject());
    //get index
    size_t indexA=(size_t)(contact->GetFixtureA()->GetUserData());
    size_t indexB=(size_t)(contact->GetFixtureB()->GetUserData());
    //get info
    Body::Info info;
    info.contact=contact;
    info.manifold.setManifold(*contact->GetManifold());
    info.manifold.points[0]=info.manifold.points[0] WORLD_PIXEL_RATIO;
    info.manifold.points[1]=info.manifold.points[1] WORLD_PIXEL_RATIO;
    //callback
    if(bodyA->cbEnd)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        info.orderCase=1;
        bodyA->cbEnd(objectB,info);
    }
    if(bodyB->cbEnd)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
        info.orderCase=2;
        bodyB->cbEnd(objectA,info);
    }
}
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    //Get body
    Body* bodyA=(Body*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Body* bodyB=(Body*)(contact->GetFixtureB()->GetBody()->GetUserData());
    //no callback?
    if( !bodyA->cbPreSolver && !bodyB->cbPreSolver ) return;
    //Get body
    Object* objectA=(bodyA->getObject());
    Object* objectB=(bodyB->getObject());
    //get index
    size_t indexA=(size_t)(contact->GetFixtureA()->GetUserData());
    size_t indexB=(size_t)(contact->GetFixtureB()->GetUserData());
    //get info
    Body::Info info;
    info.contact=contact;
    info.manifold.setManifold(*contact->GetManifold());
    info.manifold.points[0]=info.manifold.points[0] WORLD_PIXEL_RATIO;
    info.manifold.points[1]=info.manifold.points[1] WORLD_PIXEL_RATIO;

    Body::Manifold oldmf;
    oldmf.setManifold(*oldManifold);    
    oldmf.points[0]=oldmf.points[0] WORLD_PIXEL_RATIO;
    oldmf.points[1]=oldmf.points[1] WORLD_PIXEL_RATIO;
    //callback
    if(bodyA->cbPreSolver)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        info.orderCase=1;
        bodyA->cbPreSolver(objectB,info,oldmf);
    }
    if(bodyB->cbPreSolver)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
        info.orderCase=2;
        bodyB->cbPreSolver(objectA,info,oldmf);
    }   
}
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* b2impulse)
{
    //Get body
    Body* bodyA=(Body*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Body* bodyB=(Body*)(contact->GetFixtureB()->GetBody()->GetUserData());
    //no callback?
    if( !bodyA->cbPostSolver && !bodyB->cbPostSolver ) return;
    //Get body
    Object* objectA=(bodyA->getObject());
    Object* objectB=(bodyB->getObject());
    //get index
    size_t indexA=(size_t)(contact->GetFixtureA()->GetUserData());
    size_t indexB=(size_t)(contact->GetFixtureB()->GetUserData());
    //get info
    Body::Info info;
    info.contact=contact;
    info.manifold.setManifold(*contact->GetManifold());
    info.manifold.points[0]=info.manifold.points[0] WORLD_PIXEL_RATIO;
    info.manifold.points[1]=info.manifold.points[1] WORLD_PIXEL_RATIO;

    Body::Impulse impulse;
    impulse.setContactImpulse(*b2impulse);
    //callback
    if(bodyA->cbPostSolver)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        info.orderCase=1;
        bodyA->cbPostSolver(objectB,info,impulse);
    }
    if(bodyB->cbPostSolver)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
        info.orderCase=2;
        bodyB->cbPostSolver(objectA,info,impulse);
    }   
}
////////////////////////////////////////
void World::SayGoodbye( b2Joint* join )
{
    //get iterator
    auto rit=reverseJointHash.find(join);
    //delete from map
    if(rit!=reverseJointHash.end())
    {
        jointHash.erase(rit->second);
        reverseJointHash.erase(rit);
    }
}
uint World::createJoint( const b2JointDef& def )
{
    // Create Joint.
    b2Joint* pJoint = world->CreateJoint( &def );
    //add join into map
    jointHash[autoIdJoin]=pJoint;
    reverseJointHash[pJoint]=autoIdJoin;
    //next id
    ++autoIdJoin;
    //return id
    return autoIdJoin-1;
}
void World::deleteJoint(uint id)
{   
    //get join
    b2Joint* join=findB2Joint(id);
    //if is not null
    if(join)
    {
        SayGoodbye(join);
        world->DestroyJoint(join);
    }
}
b2Joint* World::findB2Joint(uint id)
{    
    //get iterator
    auto rit=jointHash.find(id);
    //get id from map
    if(rit!=jointHash.end())
    {
        return rit->second;
    }
    //else return null
    return nullptr;
    
}
uint World::findJoint(b2Joint* join)
{
    //get iterator
    auto rit=reverseJointHash.find(join);
    //get id from map
    if(rit!=reverseJointHash.end())
    {
        return rit->second;
    }
    //else return 0
    return 0;
}
/// Distance joint.
uint World::createDistanceJoint(const Object* objectA,
                                const Object* objectB,
                                const Vec2& localAnchorA, 
                                const Vec2& localAnchorB, 
                                float length,
                                float frequency,
                                float dampingRatio,
                                bool collideConnected)
{
    
    DEBUG_ASSERT(objectA);
    DEBUG_ASSERT(static_cast<const World*>(objectA->getScene())==this);
    DEBUG_ASSERT(objectB);
    DEBUG_ASSERT(static_cast<const World*>(objectB->getScene())==this);
    
    const Body* bodyA=objectA->getComponent<Body>();
    const Body* bodyB=objectB->getComponent<Body>();

    b2DistanceJointDef jointDef;
    jointDef.userData = this;
    jointDef.collideConnected = collideConnected;
    jointDef.bodyA = bodyA->body;
    jointDef.bodyB = bodyB->body;
    jointDef.localAnchorA = cast(localAnchorA PTM_RATIO );
    jointDef.localAnchorB = cast(localAnchorB PTM_RATIO );
    jointDef.length       =(    length < 0.0f 
                              ? (bodyB->getPosition() - bodyA->getPosition()).length()
                              : length
                           ) PTM_RATIO;
    jointDef.frequencyHz  = frequency;
    jointDef.dampingRatio = dampingRatio;

    return createJoint(jointDef);
}

void World::setDistanceJointLength(uint jointId,float length)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setDistanceJointLength, invalid joint type ");
        return;
    }
    // set distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    realJoint->SetLength(length  PTM_RATIO);
}
float World::getDistanceJointLength(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getDistanceJointLength, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    return realJoint->GetLength() PIXEL_RATIO;
}

void World::setDistanceJointFrequency(uint jointId,float frequency)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setDistanceJointFrequency, invalid joint type ");
        return;
    }
    // set distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    realJoint->SetFrequency( frequency );
}
float World::getDistanceJointFrequency(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getDistanceJointFrequency, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    return realJoint->GetFrequency();
}

void World::setDistanceJointDampingRatio(uint jointId,float dampingRatio)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setDistanceJointDampingRatio, invalid joint type ");
        return;
    }
    // set distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    realJoint->SetDampingRatio( dampingRatio );
}
float World::getDistanceJointDampingRatio(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getDistanceJointDampingRatio, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2DistanceJoint* realJoint = static_cast<b2DistanceJoint*>( joint );
    return realJoint->GetDampingRatio();
}

/// Revolute joint.
uint World::createRevoluteJoint(const Object* objectA, 
                                const Object* objectB,
                                const Vec2& localAnchorA, 
                                const Vec2& localAnchorB,
                                bool collideConnected)
{
    DEBUG_ASSERT(objectA);
    DEBUG_ASSERT(static_cast<const World*>(objectA->getScene())==this);
    DEBUG_ASSERT(objectB);
    DEBUG_ASSERT(static_cast<const World*>(objectB->getScene())==this);
    
    const Body* bodyA=objectA->getComponent<Body>();
    const Body* bodyB=objectB->getComponent<Body>();

    b2RevoluteJointDef jointDef;
    jointDef.userData = this;
    jointDef.collideConnected = collideConnected;
    jointDef.bodyA = bodyA->body;
    jointDef.bodyB = bodyB->body;
    jointDef.localAnchorA = cast(localAnchorA PTM_RATIO );
    jointDef.localAnchorB = cast(localAnchorB PTM_RATIO );
    jointDef.collideConnected = collideConnected;

    return createJoint(jointDef);
}

void World::setRevoluteJointLimit(uint jointId,
                                  bool enableLimit,
                                  float lowerAngle,
                                  float upperAngle )
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setRevoluteJointLimit, invalid joint type ");
        return;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    realJoint->SetLimits(Math::torad(lowerAngle),Math::torad(upperAngle));    
    realJoint->EnableLimit( enableLimit );
}
bool World::getRevoluteJointLimit(uint jointId,
                                  bool& enableLimit,
                                  float& lowerAngle,
                                  float& upperAngle )
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getRevoluteJointLimit, invalid joint type ");
        return false;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    enableLimit=realJoint->IsLimitEnabled();
    lowerAngle=Math::todeg(realJoint->GetLowerLimit());
    upperAngle=Math::todeg(realJoint->GetUpperLimit());    
    return true;
}

void World::setRevoluteJointMotor(uint jointId,
                                  bool enableMotor,
                                  float motorSpeed,
                                  float maxMotorTorque)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setRevoluteJointMotor, invalid joint type ");
        return;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    realJoint->SetMotorSpeed(motorSpeed);    
    realJoint->SetMaxMotorTorque(maxMotorTorque);    
    realJoint->EnableMotor(enableMotor);
}
bool World::getRevoluteJointMotor(uint jointId,
                                  bool& enableMotor,
                                  float& motorSpeed,
                                  float& maxMotorTorque )
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getRevoluteJointMotor, invalid joint type ");
        return false;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    enableMotor=realJoint->IsMotorEnabled();
    motorSpeed=realJoint->GetMotorSpeed();
    maxMotorTorque=realJoint->GetMaxMotorTorque();
    return true;
}

float World::getRevoluteJointAngle(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getRevoluteJointAngle, invalid joint type ");
        return 0.0;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    return realJoint->GetJointAngle();
}
float World::getRevoluteJointSpeed(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_distanceJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getRevoluteJointSpeed, invalid joint type ");
        return 0.0;
    }
    // set distance
    b2RevoluteJoint* realJoint = static_cast<b2RevoluteJoint*>( joint );
    return realJoint->GetJointSpeed();
}
////////////////////////////////////////
///WELD
uint World::createWeldJoint( const Object* objectA,
                             const Object* objectB,
                             const Vec2& localAnchorA,
                             const Vec2& localAnchorB,
                             Angle referenceAngle,
                             float frequency,
                             float dampingRatio,
                             bool collideConnected)
{
    DEBUG_ASSERT(objectA);
    DEBUG_ASSERT(static_cast<const World*>(objectA->getScene())==this);
    DEBUG_ASSERT(objectB);
    DEBUG_ASSERT(static_cast<const World*>(objectB->getScene())==this);
    
    const Body* bodyA=objectA->getComponent<Body>();
    const Body* bodyB=objectB->getComponent<Body>();
    
    b2WeldJointDef jointDef;
    jointDef.userData = this;
    jointDef.collideConnected = collideConnected;
    jointDef.bodyA = bodyA->body;
    jointDef.bodyB = bodyB->body;
    jointDef.localAnchorA   = cast(localAnchorA PTM_RATIO );
    jointDef.localAnchorB   = cast(localAnchorB PTM_RATIO );
    jointDef.referenceAngle = referenceAngle.valueRadians();
    jointDef.frequencyHz  = frequency;
    jointDef.dampingRatio = dampingRatio;
    
    return createJoint(jointDef);
}

/* TRICK */
class b2WeldJointTrick : public b2WeldJoint
{
public:
    void SetReferenceAngle(float32 rAngle) { m_referenceAngle = rAngle; }
};

void World::setWeldJointAngle(uint jointId,Angle angle)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setWeldJointAngle, invalid joint type ");
        return;
    }
    // get distance
    b2WeldJointTrick* realJoint = static_cast<b2WeldJointTrick*>( joint );
    realJoint->SetReferenceAngle(angle.valueRadians());
}

Angle World::getWeldJointAngle(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getWeldJointAngle, invalid joint type ");
        return Angle::radian(0.0f);
    }
    // get distance
    b2WeldJoint* realJoint = static_cast<b2WeldJoint*>( joint );
    return Angle::radian(realJoint->GetReferenceAngle());
}

void World::setWeldJointFrequency(uint jointId,float frequency)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setWeldJointFrequency, invalid joint type ");
        return;
    }
    // set distance
    b2WeldJoint* realJoint = static_cast<b2WeldJoint*>( joint );
    realJoint->SetFrequency(frequency);
}

float World::getWeldJointFrequency(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getWeldJointFrequency, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2WeldJoint* realJoint = static_cast<b2WeldJoint*>( joint );
    return realJoint->GetFrequency();
}

void World::setWeldJointDampingRatio(uint jointId,float dampingRatio)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setWeldJointDampingRatio, invalid joint type ");
        return;
    }
    // get distance
    b2WeldJoint* realJoint = static_cast<b2WeldJoint*>( joint );
    realJoint->SetDampingRatio(dampingRatio);
}

float World::getWeldJointDampingRatio(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_weldJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getWeldJointDampingRatio, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2WeldJoint* realJoint = static_cast<b2WeldJoint*>( joint );
    return realJoint->GetDampingRatio();
}
////////////////////////////////////////
uint World::createMouseJoint( const Object* object,
                              const Vec2& target,
                              float force,
                              float frequency,
                              float dampingRatio,
                              bool collideConnected )
{
    DEBUG_ASSERT(object);
    DEBUG_ASSERT(static_cast<const World*>(object->getScene())==this);
    
    const Body* body=object->getComponent<Body>();
    
    b2MouseJointDef jointDef;
    jointDef.userData = this;
    jointDef.collideConnected = collideConnected;
    jointDef.bodyA    = ground;
    jointDef.bodyB    = body->body;
    jointDef.target   = cast(target PTM_RATIO);
    jointDef.maxForce = force;
    jointDef.frequencyHz  = frequency;
    jointDef.dampingRatio = dampingRatio;
    
    return createJoint(jointDef);
}

void World::setMouseJointTarget(uint jointId,const Vec2& value)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setMouseJointTarget, invalid joint type ");
        return;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    realJoint->SetTarget(cast(value PTM_RATIO));
}

Vec2 World::getMouseJointTarget(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getMouseJointTarget, invalid joint type ");
        return Vec2::ZERO;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    return cast(realJoint->GetTarget()) FORCE_PIXEL_RATIO;
}

void World::setMouseJointForce(uint jointId,float force)
{
    
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setMouseJointForce, invalid joint type ");
        return;
    }
    // set distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    realJoint->SetMaxForce(force);
}

float World::getMouseJointForce(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getMouseJointForce, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    return realJoint->GetMaxForce();
}

void World::setMouseJointFrequency(uint jointId,float frequency)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setMouseJointFrequency, invalid joint type ");
        return;
    }
    // set distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    realJoint->SetFrequency(frequency);
}

float World::getMouseJointFrequency(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getMouseJointFrequency, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    return realJoint->GetFrequency();
}

void World::setMouseJointDampingRatio(uint jointId,float dampingRatio)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: setMouseJointDampingRatio, invalid joint type ");
        return;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    realJoint->SetDampingRatio(dampingRatio);
}

float World::getMouseJointDampingRatio(uint jointId)
{
    //get join
    b2Joint* joint=findB2Joint(jointId);
    // Fetch joint type.
    const b2JointType jointType = joint->GetType();
    if ( jointType != e_mouseJoint )
    {
        DEBUG_MESSAGE_IF(1,"Warning: getMouseJointDampingRatio, invalid joint type ");
        return 0.0f;
    }
    // get distance
    b2MouseJoint* realJoint = static_cast<b2MouseJoint*>( joint );
    return realJoint->GetDampingRatio();
}
////////////////////////////////////////
World::World(const Vec2& gravity):glDebugDraw(this)
                                 ,cListener(this)
                                 ,autoIdJoin(1)
                                 ,velocityIterations(8)
                                 ,positionIterations(3)
                                 ,metersUnit(1.0)
                                 ,metersInPixel(1.0)
                                 ,debugFlag(false)
{
    //create world
    world = new b2World(cast(gravity FORCE_PTM_RATIO));
    world->SetContactListener(&cListener);
    world->SetDestructionListener(this);
    //create ground body
    b2BodyDef groundDef;
    ground = world->CreateBody(&groundDef);
}
World::~World()
{
    world->DestroyBody(ground);
    delete world;
}
void World::setGravity(const Vec2& gravity)
{
    world->SetGravity(cast(gravity FORCE_PTM_RATIO));
}
Vec2 World::getGravity()
{
   return cast(world->GetGravity()) FORCE_PIXEL_RATIO;
}

void World::setMetersInPixel( float pixel )
{
	if (metersInPixel == pixel) return;
	//get gravity
	Vec2 gravity=getGravity();
	//update values
    metersUnit=1.0f/pixel;
    metersInPixel=pixel;
    //update bodies
    for(auto body = world->GetBodyList(); body; body = body->GetNext())
    {
        if(body!=ground)
        {
            auto e2body=(Body*)body->GetUserData();
            e2body->updatePixelScale(metersUnit,metersInPixel);
        }
	}
	//update gravity
	setGravity(gravity);
}

void World::resetWorld()
{
    //create bodys list
    std::vector<Body*> listBody;
    for(auto body = world->GetBodyList(); body; body = body->GetNext())
    {
        if(body!=ground)
        {
            auto e2body=(Body*)body->GetUserData();
            listBody.push_back(e2body);
        }
    }
    
    //unregister all objects
    for(auto bodies:listBody)
    {
        bodies->unregisterWorld();
    }
    
    //delete world
    delete world;
    auto gravity=getGravity();
    //create world
    world = new b2World(cast(gravity FORCE_PTM_RATIO));
    world->SetContactListener(&cListener);
    world->SetDestructionListener(this);
    //create ground body
    b2BodyDef groundDef;
    ground = world->CreateBody(&groundDef);
    
    //ri-reg all objects
    for(auto bodies:listBody)
    {
        bodies->registerWorld(this);
    }
    
}



void World::physics(float dt)
{
    world->Step(dt,velocityIterations,positionIterations);
}
void World::physicsDebugDraw(bool enable)
{
    if(enable)
    {
        glDebugDraw.SetFlags(b2Draw::e_shapeBit|
                             b2Draw::e_jointBit|
                             b2Draw::e_aabbBit|
                             b2Draw::e_pairBit|
                             b2Draw::e_centerOfMassBit);
        world->SetDebugDraw(&glDebugDraw);
        debugFlag=true;
    }
    else
    {
        world->SetDebugDraw(NULL);
        debugFlag=false;
    }
}
void World::physicsDraw(Camera* camera)
{   
    //draw if only is enable
    if(!debugFlag) return; 

    //set projection matrix 
    //NOTE: GET CAMERA, VIEWPORT
    //////////////////////////////////////////////////////////////////
    auto state=RenderContext::getRenderState();
    //change param
    RenderContext::setCullFace(DISABLE);
    //blend
    RenderContext::setBlend(true);
    RenderContext::setBlendFunction(BLEND::SRC::ALPHA, BLEND::ONE::MINUS::SRC::ALPHA);
    //texture
    RenderContext::setTexture(false);
    //clients
    RenderContext::setClientState(true, false, false, false);
    //no vbo/ibo
    RenderContext::unbindVertexBuffer();
    RenderContext::unbindIndexBuffer();
    //////////////////////////////////////////////////////////////////
    //pixel scale
    Mat4 scale;
    scale.setScale(Vec2(metersInPixel,metersInPixel));
    //camera pos
    if(camera) RenderContext::setView(camera->getGlobalMatrix());
    RenderContext::setModel(scale);
    //no texture
    world->DrawDebugData();
    //////////////////////////////////////////////////////////////////
    RenderContext::setRenderState(state);
    //////////////////////////////////////////////////////////////////
}

////////////////////////////////////////
void World::serialize(Table& table)
{
    Table& tworld=table;
    tworld.set("metersInPixel",getMetersInPixel());
    tworld.set("gravity",getGravity());
}
void World::deserialize(const Table& table)
{
    const Table& tworld=table;
    setMetersInPixel(tworld.getFloat("metersInPixel",getMetersInPixel()));
    setGravity(tworld.getVector2D("gravity",getGravity()));
}
////////////////////////////////////////


void GLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
{
    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,1.0)));
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(LINE_LOOP, 0, vertexCount);
}
void GLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    Color e2color;
    
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);

    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,0.5)));
    RenderContext::drawPrimitive(TRIANGLE_FAN, 0, vertexCount);

    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,1.0)));
    RenderContext::drawPrimitive(LINE_LOOP, 0, vertexCount);
    
}
void GLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
{
    const float32 k_segments = 16.0f;
    const int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;
    
    Vec2 vertices[vertexCount];

    for (int32 i = 0; i < k_segments; ++i)
    {
        vertices[i]= cast(center) + radius * Vec2(cosf(theta), sinf(theta));
        theta += k_increment;
    }
    
    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,1.0)));
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(LINE_LOOP, 0, vertexCount);

}
void GLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    
    Color e2color;
    const float32 k_segments = 16.0f;
    const int vertexCount=16;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 theta = 0.0f;

    Vec2 vertices[vertexCount];

    for (int32 i = 0; i < k_segments; ++i)
    {
        vertices[i]= cast(center) + radius * Vec2(cosf(theta), sinf(theta));
        theta += k_increment;
    }
    
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);

    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,0.5)));
    RenderContext::drawPrimitive(TRIANGLE_FAN, 0, vertexCount);
    
    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,1.0)));
    RenderContext::drawPrimitive(LINE_LOOP, 0, vertexCount);
    
}
void GLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    
    GLfloat vertices[] = 
    {
        p1.x,p1.y,p2.x,p2.y
    };
    RenderContext::setColor(Color::normalized(Vec4(color.r, color.g, color.b,1.0)));
    RenderContext::vertexPointer(2, GL_FLOAT, 0, vertices);
    RenderContext::drawPrimitive(LINES, 0, 2);

}
void GLDebugDraw::DrawTransform(const b2Transform& xf)
{
    b2Vec2 p1 = xf.p;
    b2Vec2 p2;
    const float32 k_axisScale = 0.4f;

    p2 = p1 + k_axisScale * xf.q.GetXAxis();
    DrawSegment(p1,p2,b2Color(1,0,0));

    p2 = p1 + k_axisScale * xf.q.GetYAxis();
    DrawSegment(p1,p2,b2Color(0,1,0));
}

