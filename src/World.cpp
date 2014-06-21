#include <stdafx.h>
#include <World.h>
#include <Body.h>
#include <Scene.h>
#include <Debug.h>

///////////////////////
using namespace Easy2D;
///////////////////////
#define PTM_RATIO *metersUnit
#define PIXEL_RATIO *metersInPixel
#define WORLD_PIXEL_RATIO *world->metersInPixel
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
    jointDef.localAnchorA = cast(localAnchorA);
    jointDef.localAnchorB = cast(localAnchorB);
    jointDef.length       = length < 0.0f ? (jointDef.bodyA ->GetWorldPoint( jointDef.localAnchorB ) - 
                                             jointDef.bodyA ->GetWorldPoint( jointDef.localAnchorA )).Length() : length;
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
    realJoint->SetLength( length );
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
    return realJoint->GetLength();
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
World::World(const Vec2& gravity):glDebugDraw(this)
                                 ,cListener(this)
                                 ,autoIdJoin(1)
                                 ,velocityIterations(8)
                                 ,positionIterations(3)
                                 ,metersUnit(1.0)
                                 ,metersInPixel(1.0)
{
    world = new b2World(cast(gravity));
    world->SetContactListener(&cListener);
    world->SetDestructionListener(this);
}
World::~World()
{
    if(world)
        delete world;
}
void World::setGravity(const Vec2& gravity)
{
    world->SetGravity(cast(gravity));
}
Vec2 World::getGravity()
{
   return cast(world->GetGravity());
}
void World::physics(float dt)
{
    world->Step(dt,velocityIterations,positionIterations);
}
void World::debugDraw(bool enable)
{
    if(enable)
    {
        glDebugDraw.SetFlags(b2Draw::e_shapeBit|
                             b2Draw::e_jointBit|
                             b2Draw::e_aabbBit|
                             b2Draw::e_pairBit|
                             b2Draw::e_centerOfMassBit);
        world->SetDebugDraw(&glDebugDraw);
    }
    else
    {
        world->SetDebugDraw(NULL);
    }
}
void World::physicsDraw(Camera* camera)
{   
    //set projection matrix 
    //NOTE: GET CAMERA, VIEWPORT
    /*
    Vec4 viewport;
    glGetFloatv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    Mat4 projection;
    projection.setOrtho(viewport.x,viewport.z, viewport.y,viewport.w, 1.0f,-1.0f);
    glLoadMatrixf(projection);
    //set view port
    //glViewport( viewport.x, viewport.y, (GLsizei)viewport.z, (GLsizei)viewport.w );
    */
    //pixel scale
    Mat4 scale;
    scale.setScale(Vec2(metersInPixel,metersInPixel));
    //camera pos
    glMatrixMode(GL_MODELVIEW);
    if(camera)
        glLoadMatrixf(camera->getGlobalMatrix().mul2D(scale));
    else
        glLoadMatrixf(scale);
    //no vbo/ibo
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  0 );
    //no texture
    glDisable( GL_TEXTURE_2D );
    world->DrawDebugData();
}

////////////////////////////////////////
#define SAVE_COLOR\
    GLfloat saveGLColor4f[4];\
    glGetFloatv(GL_CURRENT_COLOR, saveGLColor4f);
#define RESET_COLOR\
    glColor4fv(saveGLColor4f);

void GLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
{
    SAVE_COLOR
    glColor4f(color.r, color.g, color.b,1);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    RESET_COLOR
}
void GLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    SAVE_COLOR

    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glColor4f(color.r, color.g, color.b,0.5f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

    glColor4f(color.r, color.g, color.b,1);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    RESET_COLOR
}
void GLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
{
    SAVE_COLOR

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
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glColor4f(color.r, color.g, color.b,1);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

    RESET_COLOR
}
void GLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    SAVE_COLOR

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
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);

    glColor4f(color.r, color.g, color.b,0.5f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

    glColor4f(color.r, color.g, color.b,1);
    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    
    RESET_COLOR
}
void GLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    SAVE_COLOR

    GLfloat vertices[] = 
    {
        p1.x,p1.y,p2.x,p2.y
    };
    glColor4f(color.r, color.g, color.b,1);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);

    RESET_COLOR
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

