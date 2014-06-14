#include <stdafx.h>
#include <World.h>
#include <Debug.h>
#include <Body.h>

///////////////////////
using namespace Easy2D;
///////////////////////

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
    info.manifold.setManifold(*contact->GetManifold());
    //callback
    if(bodyA->cbBegin)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        bodyA->cbBegin(objectB,info);
    }
    if(bodyB->cbBegin)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
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
    info.manifold.setManifold(*contact->GetManifold());
    //callback
    if(bodyA->cbEnd)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        bodyA->cbEnd(objectB,info);
    }
    if(bodyB->cbEnd)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
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
    info.manifold.setManifold(*contact->GetManifold());
    Body::Manifold oldmf;
    oldmf.setManifold(*oldManifold);
    //callback
    if(bodyA->cbPreSolver)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        bodyA->cbPreSolver(objectB,info,oldmf);
    }
    if(bodyB->cbPreSolver)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
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
    info.manifold.setManifold(*contact->GetManifold());
    Body::Impulse impulse;
    impulse.setContactImpulse(*b2impulse);
    //callback
    if(bodyA->cbPostSolver)
    {
        info.shapeA=indexA;
        info.shapeB=indexB;
        bodyA->cbPostSolver(objectB,info,impulse);
    }
    if(bodyB->cbPostSolver)
    {
        info.shapeB=indexA;
        info.shapeA=indexB;
        bodyB->cbPostSolver(objectA,info,impulse);
    }   
}
////////////////////////////////////////
World::World(const Vec2& gravity)
{
    world = new b2World(cast(gravity));
    world->SetContactListener(new ContactListener());
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
void World::physics(float dt, uint velocityIterations, uint positionIterations)
{
    world->Step(dt,velocityIterations,positionIterations);
}
void World::enableDebugDraw()
{
    debugDraw.SetFlags(b2Draw::e_shapeBit|
                       b2Draw::e_jointBit|
                       b2Draw::e_aabbBit|
                       b2Draw::e_pairBit|
                       b2Draw::e_centerOfMassBit);
    world->SetDebugDraw(&debugDraw);
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
    //GET pos camera
    glMatrixMode(GL_MODELVIEW);
    if(camera)
        glLoadMatrixf(camera->getGlobalMatrix());
    else
        glLoadIdentity();
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

