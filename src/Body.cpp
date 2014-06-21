#include <stdafx.h>
#include <Body.h>
#include <Scene.h>
#include <Debug.h>
#include <Box2D.hpp>
///////////////////////
using namespace Easy2D;
///////////////////////
REGISTERED_COMPONENT(Body, "Body")
#define PTM_RATIO *metersUnit
#define PIXEL_RATIO *metersInPixel
///////////////////////
//init
Body::Body()
{
    world=nullptr;
    body=nullptr;
    mass = 0.0;
    metersUnit = 1.0;
    metersInPixel = 1.0;
    bodyDefinition.userData        = static_cast<void*>(this);
    bodyDefinition.position.Set(0.0f, 0.0f);
    bodyDefinition.angle           = 0.0f;
    bodyDefinition.linearVelocity.Set(0.0f, 0.0f);
    bodyDefinition.angularVelocity = 0.0f;
    bodyDefinition.linearDamping   = 0.0f;
    bodyDefinition.angularDamping  = 0.0f;
    bodyDefinition.allowSleep      = true;
    bodyDefinition.awake           = true;
    bodyDefinition.fixedRotation   = false;
    bodyDefinition.bullet          = false;
    bodyDefinition.type            = b2_dynamicBody;
    bodyDefinition.active          = true;
    bodyDefinition.gravityScale    = 1.0f;

    defaultFixture.userData    = static_cast<void*>(this);
    defaultFixture.density     = 1.0f;
    defaultFixture.friction    = 0.2f;
    defaultFixture.restitution = 0.0f;
    defaultFixture.isSensor    = false;
    defaultFixture.shape       = NULL;
}
//free
Body::~Body()
{
    if(body)
    {
        // Destroy the physics body.
        world->world->DestroyBody( body );
        //to null
        body=NULL;
    }
    /*
    //delete save fixatures
    for( auto pFixtureDef : fixturesDef )
    {
        // Destroy fixture shape.
        delete pFixtureDef->shape;
        // Destroy fixture definition.
        delete pFixtureDef;
    }*/
}
//word
void Body::registerWorld(World *wrd)
{
    //save
    world=wrd;
    metersUnit=world->metersUnit;
    metersInPixel=world->metersInPixel;
    //create body
    body=wrd->world->CreateBody(&bodyDefinition);
    //copy old fixatures
    for( auto pShapeDef : shapesDef )
    {
        // build shape
        pShapeDef.buildShape(metersUnit);

        // Create fixture.
        b2Fixture* pFixture = body->CreateFixture( &pShapeDef.fixature );

        // Push fixture.
        fixtures.push_back( pFixture );

        // Destroy fixture shape.
        delete pShapeDef.fixature.shape;

    }
    shapesDef.clear();
}
void Body::unregisterWorld()
{
    // Transfer physics body configuration back to definition.
    mass  = getMass();
    bodyDefinition.type            = (b2BodyType)getType();
    bodyDefinition.position        = cast(getPosition());
    bodyDefinition.angle           = body->GetAngle(); //rad
    bodyDefinition.linearVelocity  = cast(getLinearVelocity());
    bodyDefinition.angularVelocity = getAngularVelocity();
    bodyDefinition.linearDamping   = getLinearDamping();
    bodyDefinition.angularDamping  = getAngularDamping();
    bodyDefinition.gravityScale    = getGravityScale();
    bodyDefinition.allowSleep      = getSleepingAllowed();
    bodyDefinition.awake           = getAwake();
    bodyDefinition.fixedRotation   = getFixedAngle();
    bodyDefinition.bullet          = getBullet();
    bodyDefinition.active          = getActive();
    bodyDefinition.userData        = body->GetUserData(); //this...
    //Copy fixtures to fixture definitions.
    for( auto pFixture : fixtures )
    {
        // Create fixture definition.
        b2FixtureDef pFixtureDef ;
        pFixtureDef.density      = pFixture->GetDensity();
        pFixtureDef.friction     = pFixture->GetFriction();
        pFixtureDef.restitution  = pFixture->GetRestitution();
        pFixtureDef.isSensor     = pFixture->IsSensor();
        // Push fixture definition.
        pushShape( &pFixtureDef, pFixture->GetShape() );
    }
    fixtures.clear();
    // Destroy the physics body.
    world->world->DestroyBody( body );
    //to null
    body=nullptr;
    world=nullptr;
    //save
    metersUnit=1.0;
    metersInPixel=1.0;
}


void Body::ShapeDef::buildShape(float metersUnit)
{
    switch (type)
    {
    case b2Shape::e_circle:
        {
            auto* shape=new b2CircleShape();

            shape->m_p = cast(position  PTM_RATIO );
            shape->m_radius = radius  PTM_RATIO ;

            fixature.shape=shape;
        }
        break;
    case b2Shape::e_edge:
        {
            b2EdgeShape* shape=new b2EdgeShape();

            shape->Set(cast(points[0] PTM_RATIO ), 
                       cast(points[1] PTM_RATIO ));
            shape->m_hasVertex0      = flags & 0x01 != 0 ;
            shape->m_hasVertex3      = flags & 0x02 != 0 ;
            shape->m_vertex0         = cast(prev PTM_RATIO );
            shape->m_vertex3         = cast(next PTM_RATIO );
                
            fixature.shape=shape;
        }
        break;
    //N.b. std::vector<Vec2> points=this->points;
    case b2Shape::e_polygon:   
        {
            b2PolygonShape* shape = new b2PolygonShape();
            //update vertex
            for(auto& p:points) p=p PTM_RATIO;
            //set vertexs
            shape->Set((b2Vec2*)&points[0], points.size());
                
            fixature.shape=shape;
        }
        break;
    case b2Shape::e_chain:        
        {
            b2ChainShape* shape = new b2ChainShape();
            //update vertex
            for(auto& p:points) p=p PTM_RATIO;
            //set vertexs
            shape->CreateChain((b2Vec2*)&points[0], points.size());
            
            if(flags & 0x01 != 0)
                shape->SetPrevVertex(cast(prev PTM_RATIO));

            if(flags & 0x02 != 0)
                shape->SetNextVertex(cast(next PTM_RATIO));
                
            fixature.shape=shape;
        }
        break;
    case b2Shape::e_typeCount:
    default: break; //wrong
    }
}
size_t Body::pushShape(b2FixtureDef* fixature,  b2Shape* shape)
{
   //alloc
   shapesDef.push_back(ShapeDef());
   //get last alloc
   size_t iout=shapesDef.size()-1;
   ShapeDef& shDef=shapesDef[iout];
   //shape fixature
   shDef.fixature=*fixature;
   shDef.fixature.userData=(void*)iout;
   //parse shape
   const b2Shape* bShape=shape;

   switch (bShape->GetType())
    {	
    case b2Shape::e_circle:
    {
        b2CircleShape* shape=((b2CircleShape*)bShape);
        shDef.type=bShape->GetType();
        shDef.radius= shape->m_radius  PIXEL_RATIO;
        shDef.position= cast(shape->m_p)  PIXEL_RATIO ;
    }
    break;
    case b2Shape::e_edge:
    {
        b2EdgeShape* shape=((b2EdgeShape*)bShape);
        shDef.type=bShape->GetType();

        shDef.flags  = 0x01 * shape->m_hasVertex0;
        shDef.flags |= 0x02 * shape->m_hasVertex3;
        
        shDef.prev=cast(shape->m_vertex0) PIXEL_RATIO;
        shDef.next=cast(shape->m_vertex3) PIXEL_RATIO;

        shDef.points.resize(2);
        shDef.points[0]=cast(shape->m_vertex1) PIXEL_RATIO;
        shDef.points[1]=cast(shape->m_vertex2) PIXEL_RATIO;
    }
    break;
    case b2Shape::e_polygon:
    {
        b2PolygonShape* shape=((b2PolygonShape*)bShape);
        shDef.type=bShape->GetType();
        
        shDef.points.resize(shape->m_count);
        for(uint i=0;i!=shape->m_count;++i)
        {
            shDef.points[i]=cast(shape->m_vertices[i]) PIXEL_RATIO ;
        }
    }
    break;
    case b2Shape::e_chain:
    {
        b2ChainShape* shape=((b2ChainShape*)bShape);
        shDef.type=bShape->GetType();
        
        shDef.flags  = 0x01 * shape->m_hasPrevVertex;
        shDef.flags |= 0x02 * shape->m_hasNextVertex;
        
        shDef.prev=cast(shape->m_prevVertex) PIXEL_RATIO;
        shDef.next=cast(shape->m_nextVertex) PIXEL_RATIO;
        
        shDef.points.resize(shape->m_count);
        for(uint i=0;i!=shape->m_count;++i)
        {
            shDef.points[i]=cast(shape->m_vertices[i]) PIXEL_RATIO ;
        }
    }
    break;
    case b2Shape::e_typeCount:
    default: break; //wrong
   }
   return iout;
}

void Body::setActive(bool active)
{
    if(body)
    {
        body->SetActive(active);
        return;
    }
    bodyDefinition.active=active;
}
bool Body::getActive() const
{
    if(body)
        return body->IsActive();
    return bodyDefinition.active;
}

void Body::setAwake(bool awake)
{
    if(body)
    {
        body->SetAwake(awake);
        return;
    }
    bodyDefinition.awake=awake;
}
bool Body::getAwake() const
{
    if(body)
        return body->IsAwake();
    return bodyDefinition.awake;
}

void Body::setBullet(bool bullet)
{
    if(body)
    {
        body->SetBullet(bullet);
        return;
    }
    bodyDefinition.bullet=bullet;
}
bool Body::getBullet()
{
    if(body)
        return body->IsBullet();
    return bodyDefinition.bullet;
}

void Body::setType(Type type)
{
    if(body)
    {
        body->SetType((b2BodyType)type);
        return;
    }
    bodyDefinition.type=(b2BodyType)type;
}
Body::Type Body::getType() const
{
    if(body)
        return (Body::Type)body->GetType();
    return (Body::Type)bodyDefinition.type;
}

float Body::getMass() const
{
    if(body)
        return body->GetMass();
    return mass;
}

//get world
Vec2 Body::getWorldCenter() const
{
    if(body)
        return cast(body->GetWorldCenter()) PIXEL_RATIO ;
    return Vec2::ZERO;
}
Vec2 Body::getWorldPoint(const Vec2& local) const
{
    if(body)
        return cast(body->GetWorldPoint(cast(local))) PIXEL_RATIO ;
    return Vec2::ZERO;
}
//get local
Vec2 Body::getLocalVector(const Vec2& worldVector) const
{
    if(body)
        return cast(body->GetLocalVector(cast(worldVector))) PIXEL_RATIO ;
    return Vec2::ZERO;
}
Vec2 Body::getLocalPoint(const Vec2& worldPoint) const
{
    if(body)
        return cast(body->GetLocalPoint(cast(worldPoint))) PIXEL_RATIO ;
    return Vec2::ZERO;
}
Vec2 Body::getLocalCenter() const
{
    if(body)
        return cast(body->GetLocalCenter()) PIXEL_RATIO ;
    return Vec2::ZERO;
}


/**
* applay
*/
void Body::applyForce(const Vec2& force, const Vec2& point, bool wake)
{
    if(body) body->ApplyForce(cast(force PTM_RATIO ),cast(point PTM_RATIO ),wake);
}
void Body::applyForceToCenter(const Vec2& force, bool wake)
{
    if(body) body->ApplyForceToCenter(cast(force PTM_RATIO ),wake);
}
void Body::applyTorque(float torque, bool wake)
{
    if(body) body->ApplyTorque(torque,wake);
}
void Body::applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake)
{
    if(body) body->ApplyLinearImpulse(cast(impulse PTM_RATIO ),cast(point PTM_RATIO ),wake);
}
void Body::applyAngularImpulse(float impulse, bool wake)
{
    if(body) body->ApplyAngularImpulse(impulse,wake);
}
//
void Body::setLinearVelocity(const Vec2& vl)
{
    if(body)
    {
        body->SetLinearVelocity(cast(vl PTM_RATIO ));
        return;
    }
    bodyDefinition.linearVelocity=cast(vl PTM_RATIO );
}
Vec2 Body::getLinearVelocity() const
{
    if(body)
        return cast(body->GetLinearVelocity()) PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) PIXEL_RATIO ;
}

Vec2 Body::getLinearVelocityFromWorldPoint(const Vec2& world) const
{
    if(body)
        return cast(body->GetLinearVelocityFromWorldPoint(cast(world))) PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) PIXEL_RATIO ;
}
Vec2 Body::getLinearVelocityFromLocalPoint(const Vec2& world) const
{
    if(body)
        return cast(body->GetLinearVelocityFromLocalPoint(cast(world))) PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) PIXEL_RATIO ;
}

void Body::setLinearDamping(float linearDamping)
{
    if(body)
    {
        body->SetLinearDamping(linearDamping PTM_RATIO );
        return;
    }
    bodyDefinition.linearDamping=linearDamping PTM_RATIO ;
}
float Body::getLinearDamping() const
{
    if(body)
        return body->GetLinearDamping() PIXEL_RATIO ;
    return bodyDefinition.linearDamping PIXEL_RATIO ;
}

void Body::setAngularVelocity(float angularVelocity)
{
    if(body)
    {
        body->SetAngularVelocity(angularVelocity);
        return;
    }
    bodyDefinition.angularVelocity=angularVelocity;
}
float Body::getAngularVelocity() const
{
    if(body)
        return body->GetAngularVelocity();
    return bodyDefinition.angularVelocity;
}

void Body::setAngularDamping(float angularDamping)
{

    if(body)
    {
        body->SetAngularDamping(angularDamping);
        return;
    }
    bodyDefinition.angularDamping=angularDamping;
}
float Body::getAngularDamping() const
{
    if(body)
        return body->GetAngularDamping();
    return bodyDefinition.angularDamping;
}


void Body::setAngle(float angle)
{
    if(body)
    {
        body->SetTransform(body->GetPosition(), Math::torad(angle));
        return;
    }
    bodyDefinition.angle=Math::torad(angle);
}
float Body::getAngle() const
{
    if(body)
        return Math::todeg(body->GetAngle());
    return Math::todeg(bodyDefinition.angle);
}

void Body::setPosition(const Vec2& pos)
{
    if(body)
    {
        body->SetTransform(cast(pos PTM_RATIO ), body->GetAngle());
        return;
    }
    bodyDefinition.position=cast(pos PTM_RATIO );
}
Vec2 Body::getPosition() const
{
    if(body)
        return cast(body->GetPosition()) PIXEL_RATIO ;
    return cast(bodyDefinition.position) PIXEL_RATIO ;
}

void Body::setFixedAngle(bool fixed)
{
    if(body)
    {
        body->SetFixedRotation(fixed);
        return;
    }
    bodyDefinition.fixedRotation=fixed;
}
bool Body::getFixedAngle() const
{
    if(body)
        return body->IsFixedRotation();
    return bodyDefinition.fixedRotation;
}


void Body::setGravityScale(float gravityScale)
{
    if(body)
    {
        body->SetGravityScale(gravityScale PTM_RATIO );
        return;
    }
    bodyDefinition.gravityScale=gravityScale PTM_RATIO;
}
float Body::getGravityScale() const
{
    if(body)
        return body->GetGravityScale() PIXEL_RATIO ;
    return bodyDefinition.gravityScale PIXEL_RATIO ;
}

void Body::setSleepingAllowed(bool sleepingAllowed)
{
    if(body)
    {
        body->SetSleepingAllowed(sleepingAllowed);
        return;
    }
    bodyDefinition.allowSleep=sleepingAllowed;
}
bool Body::getSleepingAllowed() const
{
    if(body)
        return body->IsSleepingAllowed();
    return bodyDefinition.allowSleep;
}


uint Body::addFixatureShape(b2FixtureDef* pFixtureDef,   b2Shape* pShape)
{
    if ( body )
    {
        //set shape
        pFixtureDef->shape = pShape;
        //save id
        pFixtureDef->userData=(void*)fixtures.size();
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        //return id
        return fixtures.size()-1;
    }
    //push
    size_t iout=pushShape(pFixtureDef,pShape);
    // Destroy shape and fixture.
    delete pShape;
    delete pFixtureDef;
    //ret id
    return iout;
}

Shape Body::createCircleCollisionShape(float radius, const Vec2& pos)
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2CircleShape* pShape = new b2CircleShape();
    pShape->m_p = cast(pos  PTM_RATIO );
    pShape->m_radius = radius  PTM_RATIO ;
    //return id
    return addFixatureShape(pFixtureDef,pShape);
}
Shape Body::createBoxCollisionShape(const Vec2& size, const Vec2& pos, float angle)
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2PolygonShape* pShape = new b2PolygonShape();
    pShape->SetAsBox(size.x  PTM_RATIO , 
                     size.y  PTM_RATIO , 
                     cast(pos  PTM_RATIO ),
                     Math::torad(angle));
    //return id
    return addFixatureShape(pFixtureDef,pShape);
}

Shape Body::createPolygonCollisionShape( const std::vector<Vec2>& argpoints )
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2PolygonShape* pShape = new b2PolygonShape();
    //update vertex
    std::vector<Vec2> points=argpoints;
    for(auto& p:points) p=p PTM_RATIO;
    //debug
    DEBUG_ASSERT_MSG( points.size() >= 3 && points.size() <= b2_maxPolygonVertices ,
                      "createChainCollisionShape error: "
                      "points numbers must to be between 3 and "+
                      String::toString(b2_maxPolygonVertices)+
                      ", you added "+
                      String::toString(points.size()));

    //set vertexs
    pShape->Set((b2Vec2*)&points[0], points.size());

    //return id
    return addFixatureShape(pFixtureDef,pShape);

}

Shape Body::createChainCollisionShape( const std::vector<Vec2>& argpoints )
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2ChainShape* pShape = new b2ChainShape();
    //
    std::vector<Vec2> points=argpoints;
    for(auto& p:points)  p=p PTM_RATIO;
    //
    pShape->CreateChain((b2Vec2*)&points[0], points.size());

    //return id
    return addFixatureShape(pFixtureDef,pShape);

}

Shape Body::createChainCollisionShape( const std::vector<Vec2>& argpoints,
                                       bool startp ,
                                       const Vec2& adjacentStartPoint,
                                       bool endp,
                                       const Vec2& adjacentEndPoint )
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2ChainShape* pShape = new b2ChainShape();
    //
    std::vector<Vec2> points=argpoints;
    for(auto& p:points)
        p=p PTM_RATIO;
    //
    pShape->CreateChain((b2Vec2*)&points[0], points.size());

    if(startp)
        pShape->SetPrevVertex(cast(adjacentStartPoint PTM_RATIO ));
    if(endp)
        pShape->SetNextVertex(cast(adjacentEndPoint PTM_RATIO ));

    //return id
    return addFixatureShape(pFixtureDef,pShape);

}
Shape Body::createEdgeCollisionShape( const Vec2& localPositionStart,  const Vec2& localPositionEnd)
{

    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2EdgeShape* pShape = new b2EdgeShape();
    pShape->Set(cast(localPositionStart  PTM_RATIO ), 
                cast(localPositionEnd PTM_RATIO ));
    //return id
    return addFixatureShape(pFixtureDef,pShape);


}
Shape Body::createEdgeCollisionShape( const Vec2& localPositionStart,
                                      const Vec2& localPositionEnd,
                                      const bool hasAdjacentLocalPositionStart,
                                      const Vec2& adjacentLocalPositionStart,
                                      const bool hasAdjacentLocalPositionEnd,
                                      const Vec2& adjacentLocalPositionEnd )
{


    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2EdgeShape* pShape = new b2EdgeShape();
    pShape->Set(cast(localPositionStart PTM_RATIO ), 
                cast(localPositionEnd PTM_RATIO ));
    pShape->m_hasVertex0      = hasAdjacentLocalPositionStart;
    pShape->m_hasVertex3      = hasAdjacentLocalPositionEnd;
    pShape->m_vertex0         = cast(adjacentLocalPositionStart PTM_RATIO );
    pShape->m_vertex3         = cast(adjacentLocalPositionEnd PTM_RATIO );
    //return id
    return addFixatureShape(pFixtureDef,pShape);

}

void Body::setCollisionShapeDensity(Shape index,float density)
{
    if(body)
    {
        fixtures[index]->SetDensity(density);
        return;
    }
    shapesDef[index].fixature.density=density;
}
float Body::getCollisionShapeDensity(Shape index)
{
    if(body)
        return fixtures[index]->GetDensity();
    return shapesDef[index].fixature.density;
}

void Body::setCollisionShapeFriction(Shape index,float friction)
{
    if(body)
    {
        fixtures[index]->SetFriction(friction);
        return;
    }
    shapesDef[index].fixature.friction=friction;
}
float Body::getCollisionShapeFriction(Shape index) const
{
    if(body)
        return fixtures[index]->GetFriction();
    return shapesDef[index].fixature.friction;
}

void Body::setCollisionShapeRestitution(Shape index,float restitution)
{
    if(body)
    {
        fixtures[index]->SetRestitution(restitution);
        return;
    }
    shapesDef[index].fixature.restitution=restitution;
}
float Body::getCollisionShapeRestitution(Shape index) const
{
    if(body)
        return fixtures[index]->GetRestitution();
    return shapesDef[index].fixature.restitution;
}

void Body::setCollisionShapeIsSensor(Shape index,bool sensor)
{
    if(body)
    {
        fixtures[index]->SetSensor(sensor);
        return;
    }
    shapesDef[index].fixature.isSensor=sensor;
}
bool Body::getCollisionShapeIsSensor(Shape index) const
{
    if(body)
        return fixtures[index]->IsSensor();
    return shapesDef[index].fixature.isSensor;
}


void Body::onRun(float dt) 
{
    Transform2D t2d;
    t2d.position=getPosition();
    t2d.alpha=getAngle();
    t2d.scale=getObject()->getScale();
    getObject()->setTransform(t2d);
}
void Body::onSetScene(Scene* scene)
{
    if(!world)
        registerWorld((World*)scene);
    DEBUG_CODE(
    else
         DEBUG_ASSERT(world==((World*)scene))
    );
}
void Body::onEraseScene()
{
    if(world)
        unregisterWorld();
}
//object
void Body::onSetObject(Object* object)
{
    if(object->getScene() && !world)
        registerWorld((World*)getObject()->getScene());
}
void Body::onEraseObject()
{
    if(world)
        unregisterWorld();
}

static inline void shapeSerialize(float metersInPixel,
                                  Table& tshape,
                                  const b2Shape* bShape)
{
    Table& tgeometry=tshape.createTable("geometry");
    switch (bShape->GetType())
    {	
    case b2Shape::e_circle:
    {
        b2CircleShape* shape=((b2CircleShape*)bShape);
        tgeometry.set("type","circle");
        tgeometry.set("radius", shape->m_radius  PIXEL_RATIO );
        tgeometry.set("position", cast(shape->m_p)  PIXEL_RATIO );
    }
    break;
    case b2Shape::e_edge:
    {
        tgeometry.set("type","edge");
        b2EdgeShape* shape=((b2EdgeShape*)bShape);
        if(shape->m_hasVertex0)
            tgeometry.set("v0", cast(shape->m_vertex0) PIXEL_RATIO );

        tgeometry.set("v1", cast(shape->m_vertex1) PIXEL_RATIO );
        tgeometry.set("v2", cast(shape->m_vertex2) PIXEL_RATIO );

        if(shape->m_hasVertex3)
            tgeometry.set("v3", cast(shape->m_vertex3) PIXEL_RATIO );
    }
    break;
    case b2Shape::e_polygon:
    {
        tgeometry.set("type","polygon");
        Table& tpoints=tgeometry.createTable("points");
        b2PolygonShape* shape=((b2PolygonShape*)bShape);

        for(uint i=0;i!=shape->m_count;++i)
        {
            tpoints.set(cast(shape->m_vertices[i]) PIXEL_RATIO );
        }
    }
    break;
    case b2Shape::e_chain:
    {
        tgeometry.set("type","chain");

        Table& tpoints=tgeometry.createTable("points");
        b2ChainShape* shape=((b2ChainShape*)bShape);
        
        if(shape->m_hasPrevVertex)
            tgeometry.set("start", cast(shape->m_prevVertex) PIXEL_RATIO );
        if(shape->m_hasNextVertex)
            tgeometry.set("end", cast(shape->m_nextVertex) PIXEL_RATIO );

        for(uint i=0;i!=shape->m_count;++i)
        {
            tpoints.set(cast(shape->m_vertices[i]) PIXEL_RATIO );
        }
    }
    break;
    case b2Shape::e_typeCount:
    default: break; //wrong
    }
}
static inline Body::Type getBodyType(const String& name)
{
    if(name=="dynamic") return Body::DINAMIC;
    if(name=="kinematic") return Body::KINEMATIC;
    return Body::STATIC;
}
static inline String getBodyType(Body::Type type)
{
    if(type==b2_dynamicBody) return "dynamic";
    if(type==b2_kinematicBody) return "kinematic";
    return "static";
}

void Body::serialize(Table& table)
{
    Table& rbody=table.createTable(getComponentName());
    rbody.set("position",getPosition());
    rbody.set("rotation",getAngle());
    rbody.set("linearVelocity",getLinearVelocity());
    rbody.set("angularVelocity",getAngularVelocity());
    rbody.set("gravityScale",getGravityScale());
    rbody.set("allowSleep",getSleepingAllowed());
    rbody.set("awake",getAwake());
    rbody.set("fixedRotation",getFixedAngle());
    rbody.set("bullet",getBullet());
    rbody.set("active",getActive()); 
    rbody.set("type",getBodyType(getType())); 
    //TODO: SAVE FIXATURES
    if(body)
    {
        Table& tshapes=rbody.createTable("shapes");
        for( auto pFixture : fixtures )
        {
            Table& tshape=tshapes.createTable();
            tshape.set("density",pFixture->GetDensity());
            tshape.set("friction",pFixture->GetFriction());
            tshape.set("restitution",pFixture->GetRestitution());
            tshape.set("sensor",pFixture->IsSensor());
            //shape      
            shapeSerialize(metersInPixel,
                           tshape,
                           pFixture->GetShape());
        }
    }
    else
    {
        Table& tshapes=rbody.createTable("shapes");
        for( auto pShapeDef : shapesDef )
        {
            Table& tshape=tshapes.createTable();
            tshape.set("density",pShapeDef.fixature.density);
            tshape.set("friction",pShapeDef.fixature.friction);
            tshape.set("restitution",pShapeDef.fixature.restitution);
            tshape.set("sensor",pShapeDef.fixature.isSensor);
            //shape      
            shapeSerialize(metersInPixel,
                           tshape,
                           pShapeDef.fixature.shape);
        }
        /*
        for( auto pFixDef : fixturesDef )
        {
            Table& tshape=tshapes.createTable();
            tshape.set("density",pFixDef->density);
            tshape.set("friction",pFixDef->friction);
            tshape.set("restitution",pFixDef->restitution);
            tshape.set("sensor",pFixDef->isSensor);
            //shape      
            shapeSerialize(metersInPixel,
                           tshape,
                           pFixDef->shape);
        }*/
    }
}

void Body::deserialize(const Table& table)
{
    setPosition( table.getVector2D("position", getPosition()) );
    setAngle( table.getFloat("rotation", getAngle()) );
    setLinearVelocity( table.getVector2D("linearVelocity", getLinearVelocity()) );
    setAngularVelocity( table.getFloat("angularVelocity", getAngularVelocity()) );
    setGravityScale( table.getFloat("gravityScale",getGravityScale()) );
    setSleepingAllowed( table.getFloat("allowSleep", getSleepingAllowed()) );
    setAwake( table.getFloat("awake", getAwake()) );
    setFixedAngle( table.getFloat("fixedRotation", getFixedAngle()) );
    setBullet( table.getFloat("bullet", getBullet()) );
    setActive( table.getFloat("active", getActive()) );
    setType( getBodyType(table.getString("type", getBodyType(getType()))) );


    if(table.existsAsType("shapes",Table::TABLE))
    {
        const Table& shapes=table.getConstTable("shapes");
        for(auto ashape:shapes)
        {
            DEBUG_ASSERT(ashape.second->asType(Table::TABLE));
            const Table& shape=ashape.second->get<Table>();
            const Table& geometry=shape.getConstTable("geometry");
            const String& type=geometry.getString("type","none");
            uint idshape=0;

            if(type=="circle")
            {
                idshape=createCircleCollisionShape(
                    geometry.getFloat("radius"),
                    geometry.getVector2D("position")
                );
            }
            else if(type=="edge")
            {
                Vec2 v1=geometry.getVector2D("v1");
                Vec2 v2=geometry.getVector2D("v2");
                Vec2 v0=geometry.getVector2D("v0");
                Vec2 v3=geometry.getVector2D("v3");
                bool bv0=geometry.existsAsType("v0",Table::VECTOR2D);
                bool bv3=geometry.existsAsType("v3",Table::VECTOR2D);
                idshape=createEdgeCollisionShape(v1,v2,
                                                 bv0,v0,
                                                 bv3,v3);
            }
            else if(type=="box")
            {
                
                DEBUG_ASSERT(geometry.existsAsType("scale",Table::VECTOR2D));
                Vec2 scale=geometry.getVector2D("scale");
                Vec2 position=geometry.getVector2D("position");
                float angle=geometry.getFloat("angle");
                idshape=createBoxCollisionShape(scale,position,angle);
            }
            else if(type=="polygon")
            {
                
                DEBUG_ASSERT(geometry.existsAsType("points",Table::TABLE));
                const Table& points=geometry.getConstTable("points");
                std::vector<Vec2> vp;
                for(auto point:points)
                {
                    DEBUG_ASSERT(point.second->asType(Table::VECTOR2D));
                    vp.push_back(point.second->get<Vec2>());
                }
                idshape=createPolygonCollisionShape(vp);
            }
            else if(type=="chain")
            {
                
                DEBUG_ASSERT(geometry.existsAsType("points",Table::TABLE));
                const Table& points=geometry.getConstTable("points");
                std::vector<Vec2> vp;
                for(auto point:points)
                {
                    DEBUG_ASSERT(point.second->asType(Table::VECTOR2D));
                    vp.push_back(point.second->get<Vec2>());
                }
                
                Vec2 vstart=geometry.getVector2D("start");
                Vec2 vend=geometry.getVector2D("end");
                bool bstart=geometry.existsAsType("start",Table::VECTOR2D);
                bool bend=geometry.existsAsType("end",Table::VECTOR2D);

                idshape=createChainCollisionShape(vp,bstart,vstart,
                                                     bend,vend);
            }
            else
            {
                DEBUG_ASSERT(0);
            }
            setCollisionShapeDensity(idshape,shape.getFloat("density",defaultFixture.density));
            setCollisionShapeFriction(idshape,shape.getFloat("friction",defaultFixture.friction));
            setCollisionShapeRestitution(idshape,shape.getFloat("restitution",defaultFixture.restitution));
            setCollisionShapeIsSensor(idshape,shape.getFloat("sensor",defaultFixture.isSensor));
        }
    }
}