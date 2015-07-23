#include <stdafx.h>
#include <Body.h>
#include <Scene.h>
#include <Debug.h>
#include <Box2D.hpp>
///////////////////////
using namespace Easy2D;
///////////////////////
#define PTM_RATIO *metersUnit
#define PIXEL_RATIO *metersInPixel

#ifdef APPLAY_RATIO_BODY_ON_FORCE
    #define FORCE_PTM_RATIO *metersUnit
    #define FORCE_PIXEL_RATIO *metersInPixel
#else
    #define FORCE_PTM_RATIO 
    #define FORCE_PIXEL_RATIO
#endif

#ifdef UNSAFE_BODY_SCALE
    #define SAFE_SCALE(x)
#else
    #define SCALE_EPSILON 1e-6
    #define SAFE_SCALE(x) x
#endif

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
    //////////////////
    idShapeGen=0;
    //////////////////
    enableScale = false;
	isAbsoluteScale = false;
    lastScale=Vec2::ONE;
    //////////////////
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
        body=nullptr;
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
    //set def by ratio
    setPosition(cast(bodyDefinition.position));
    setLinearVelocity(cast(bodyDefinition.linearVelocity));
    setLinearDamping(bodyDefinition.linearDamping);
    setGravityScale(bodyDefinition.gravityScale);
    //copy old fixatures
    for( auto pShapeDef : shapesDef )
    {
        // build shape
        pShapeDef.second.buildShape(metersUnit);

        // Create fixture.
        b2Fixture* pFixture = body->CreateFixture( &pShapeDef.second.fixature );

        // Push fixture.
        fixtures[pShapeDef.first] = pFixture ;

        // Destroy fixture shape.
        delete pShapeDef.second.fixature.shape;

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
        pFixtureDef.density      = pFixture.second->GetDensity();
        pFixtureDef.friction     = pFixture.second->GetFriction();
        pFixtureDef.restitution  = pFixture.second->GetRestitution();
        pFixtureDef.isSensor     = pFixture.second->IsSensor();
		pFixtureDef.filter	     = pFixture.second->GetFilterData();
        // Push fixture definition.
        pushShape( pFixture.first, &pFixtureDef, pFixture.second->GetShape() );
    }
    fixtures.clear();
    // Destroy the physics body.
    world->world->DestroyBody( body );
    //to null
    body=nullptr;
    world=nullptr;
    //save
    metersUnit=1.0f;
    metersInPixel=1.0f;
}


void  Body::updatePixelScale(float mUnit,float mPixel)
{
    if(!body) return;
    if(mUnit==this->metersUnit) return;
    //form to
    float metersUnit= mUnit * this->metersInPixel /* or / this->metersUnit */;
    if(metersUnit==1.0f) return;
    //save new unit
    this->metersUnit=mUnit;
    this->metersInPixel=mPixel;
    //change pos
    b2Vec2 position=body->GetPosition();
    position.x = position.x PTM_RATIO;
    position.y = position.y PTM_RATIO;
    body->SetTransform(position, body->GetAngle());
    //for all fixture
    for( auto pFixture : fixtures )
    {
        switch (pFixture.second->GetType())
        {
            case b2Shape::e_circle:
            {
                auto* shape=(b2CircleShape*)pFixture.second->GetShape();
                
                shape->m_p.x = shape->m_p.x  PTM_RATIO ;
                shape->m_p.y = shape->m_p.y  PTM_RATIO ;
                shape->m_radius = shape->m_radius  PTM_RATIO ;
            }
            break;
            case b2Shape::e_edge:
            {
                auto* shape=(b2EdgeShape*)pFixture.second->GetShape();
                
                shape->m_vertex0.x = shape->m_vertex0.x PTM_RATIO ;
                shape->m_vertex0.y = shape->m_vertex0.y PTM_RATIO ;
                
                shape->m_vertex1.x = shape->m_vertex1.x PTM_RATIO ;
                shape->m_vertex1.y = shape->m_vertex1.y PTM_RATIO ;
                
                shape->m_vertex2.x = shape->m_vertex2.x PTM_RATIO ;
                shape->m_vertex2.y = shape->m_vertex2.y PTM_RATIO ;
                
                shape->m_vertex3.x = shape->m_vertex3.x PTM_RATIO ;
                shape->m_vertex3.y = shape->m_vertex3.y PTM_RATIO ;
            }
            break;
            //N.b. std::vector<Vec2> points=this->points;
            case b2Shape::e_polygon:
            {
                auto* shape=(b2PolygonShape*)pFixture.second->GetShape();
                //update vertexs
                for(int i=0;i<shape->m_count;++i)
                {
                    shape->m_vertices[i].x=shape->m_vertices[i].x PTM_RATIO;
                    shape->m_vertices[i].y=shape->m_vertices[i].y PTM_RATIO;
                }
            }
            break;
            case b2Shape::e_chain:
            {
                auto* shape=(b2ChainShape*)pFixture.second->GetShape();
                //update vertexs
                for(int i=0;i<shape->m_count;++i)
                {
                    shape->m_vertices[i].x=shape->m_vertices[i].x PTM_RATIO;
                    shape->m_vertices[i].y=shape->m_vertices[i].y PTM_RATIO;
                }
                
                shape->m_prevVertex.x = shape->m_prevVertex.x PTM_RATIO;
                shape->m_prevVertex.y = shape->m_prevVertex.y PTM_RATIO;
                
                shape->m_nextVertex.x = shape->m_nextVertex.x PTM_RATIO;
                shape->m_nextVertex.y = shape->m_nextVertex.y PTM_RATIO;
            }
            break;
            case b2Shape::e_typeCount:
            default: break;
        }
    }
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
            shape->m_hasVertex0      = (flags & 0x01) != 0 ;
            shape->m_hasVertex3      = (flags & 0x02) != 0 ;
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
            shape->Set((b2Vec2*)&points[0], (uint)points.size());
                
            fixature.shape=shape;
        }
        break;
    case b2Shape::e_chain:        
        {
            b2ChainShape* shape = new b2ChainShape();
            //update vertex
            for(auto& p:points) p=p PTM_RATIO;
            //set vertexs
            shape->CreateChain((b2Vec2*)&points[0], (uint)points.size());
            
            if((flags & 0x01) != 0)
                shape->SetPrevVertex(cast(prev PTM_RATIO));

            if((flags & 0x02) != 0)
                shape->SetNextVertex(cast(next PTM_RATIO));
                
            fixature.shape=shape;
        }
        break;
    case b2Shape::e_typeCount:
    default: break; //wrong
    }
}
void Body::pushShape(Shape index,b2FixtureDef* fixature,  b2Shape* shape)
{
   //alloc
   shapesDef[index]=ShapeDef();
   //get last alloc
   ShapeDef& shDef=shapesDef[index];
   //shape fixature
   shDef.fixature=*fixature;
   shDef.fixature.userData=(void*)index;
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
}
Shape Body::addFixatureShape(b2FixtureDef* pFixtureDef,   b2Shape* pShape)
{
    //create id
    Shape iout=newShapeId();
    //set values
    if ( body )
    {
        //set shape
        pFixtureDef->shape = pShape;
        //save id
        pFixtureDef->userData=(void*)fixtures.size();
        // Create and push fixture.
        fixtures[iout]= body->CreateFixture( pFixtureDef ) ;
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        //return id
        return iout;
    }
    //push
    pushShape(iout,pFixtureDef,pShape);
    // Destroy shape and fixture.
    delete pShape;
    delete pFixtureDef;
    //ret id
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
bool Body::getBullet() const
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
    if(body) body->ApplyForce(cast(force FORCE_PTM_RATIO ),cast(point FORCE_PTM_RATIO ),wake);
}
void Body::applyForceToCenter(const Vec2& force, bool wake)
{
    if(body) body->ApplyForceToCenter(cast(force FORCE_PTM_RATIO ),wake);
}
void Body::applyTorque(float torque, bool wake)
{
    if(body) body->ApplyTorque(torque,wake);
}
void Body::applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake)
{
    if(body) body->ApplyLinearImpulse(cast(impulse FORCE_PTM_RATIO ),cast(point FORCE_PTM_RATIO ),wake);
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
        body->SetLinearVelocity(cast(vl FORCE_PTM_RATIO ));
        return;
    }
    bodyDefinition.linearVelocity=cast(vl FORCE_PTM_RATIO );
}
Vec2 Body::getLinearVelocity() const
{
    if(body)
        return cast(body->GetLinearVelocity()) FORCE_PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) FORCE_PIXEL_RATIO ;
}

Vec2 Body::getLinearVelocityFromWorldPoint(const Vec2& world) const
{
    if(body)
        return cast(body->GetLinearVelocityFromWorldPoint(cast(world))) FORCE_PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) FORCE_PIXEL_RATIO ;
}
Vec2 Body::getLinearVelocityFromLocalPoint(const Vec2& world) const
{
    if(body)
        return cast(body->GetLinearVelocityFromLocalPoint(cast(world))) FORCE_PIXEL_RATIO ;
    return cast(bodyDefinition.linearVelocity) FORCE_PIXEL_RATIO ;
}

void Body::setLinearDamping(float linearDamping)
{
    if(body)
    {
        body->SetLinearDamping(linearDamping FORCE_PTM_RATIO );
        return;
    }
    bodyDefinition.linearDamping=linearDamping FORCE_PTM_RATIO ;
}
float Body::getLinearDamping() const
{
    if(body)
        return body->GetLinearDamping() FORCE_PIXEL_RATIO ;
    return bodyDefinition.linearDamping FORCE_PIXEL_RATIO ;
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
        body->SetGravityScale(gravityScale FORCE_PTM_RATIO );
        return;
    }
    bodyDefinition.gravityScale=gravityScale FORCE_PTM_RATIO;
}
float Body::getGravityScale() const
{
    if(body)
        return body->GetGravityScale() FORCE_PIXEL_RATIO ;
    return bodyDefinition.gravityScale FORCE_PIXEL_RATIO ;
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
/*
 * Groups
 */
void Body::setShapeMembershipMask(Shape index, byte2 categoryBits)
{
	if (body)
	{
		b2Filter filter = fixtures[index]->GetFilterData();
		filter.categoryBits = categoryBits;
		fixtures[index]->SetFilterData(filter);
		return;
	}
	shapesDef[index].fixature.filter.categoryBits = categoryBits;
}
byte2 Body::getShapeMembershipMask(Shape index) const
{
	if (body)
	{
		
		b2Filter filter = fixtures.find(index)->second->GetFilterData();
		return filter.categoryBits;
	}
	return shapesDef.find(index)->second.fixature.filter.categoryBits;
}

void Body::setShapeToCollideMask(Shape index, byte2 maskBits)
{
	if (body)
	{
		b2Filter filter = fixtures[index]->GetFilterData();
		filter.maskBits = maskBits;
		fixtures[index]->SetFilterData(filter);
		return;
	}
	shapesDef[index].fixature.filter.maskBits = maskBits;
}
byte2 Body::getShapeToCollideMask(Shape index) const
{
	if (body)
	{

		b2Filter filter = fixtures.find(index)->second->GetFilterData();
		return filter.maskBits;
	}
	return shapesDef.find(index)->second.fixature.filter.maskBits;
}

void Body::setShapeGroupMask(Shape index, byte2 groupIndex)
{

	if (body)
	{
		b2Filter filter = fixtures[index]->GetFilterData();
		filter.groupIndex = groupIndex;
		fixtures[index]->SetFilterData(filter);
		return;
	}
	shapesDef[index].fixature.filter.groupIndex = groupIndex;
}
byte2 Body::getShapeGroupMask(Shape index) const
{
	if (body)
	{

		b2Filter filter = fixtures.find(index)->second->GetFilterData();
		return filter.groupIndex;
	}
	return shapesDef.find(index)->second.fixature.filter.groupIndex;
}
/*
 * Shapes
 */
Shape Body::createCircleShape(float radius, const Vec2& pos)
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2CircleShape* pShape = new b2CircleShape();
    pShape->m_p = cast(pos  PTM_RATIO );
    pShape->m_radius = radius  PTM_RATIO ;
    //return id
    return addFixatureShape(pFixtureDef,pShape);
}

Shape Body::createBoxShape(const Vec2& size, const Vec2& pos, float angle)
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

Shape Body::createPolygonShape( const std::vector<Vec2>& argpoints )
{
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2PolygonShape* pShape = new b2PolygonShape();
    //update vertex
    std::vector<Vec2> points=argpoints;
    for(auto& p:points) p=p PTM_RATIO;
    //debug
    DEBUG_ASSERT_MSG( points.size() >= 3 && points.size() <= b2_maxPolygonVertices ,
                      "createPolygonShape error: "
                      "points numbers must to be between 3 and "+
                      String::toString(b2_maxPolygonVertices)+
                      ", you added "+
                      String::toString(points.size()));

    //set vertexs
    pShape->Set((b2Vec2*)&points[0], points.size());

    //return id
    return addFixatureShape(pFixtureDef,pShape);

}

Shape Body::createChainShape( const std::vector<Vec2>& argpoints )
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

Shape Body::createChainShape( const std::vector<Vec2>& argpoints,
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

Shape Body::createEdgeShape( const Vec2& localPositionStart,  const Vec2& localPositionEnd)
{

    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2EdgeShape* pShape = new b2EdgeShape();
    pShape->Set(cast(localPositionStart  PTM_RATIO ), 
                cast(localPositionEnd PTM_RATIO ));
    //return id
    return addFixatureShape(pFixtureDef,pShape);


}

Shape Body::createEdgeShape( const Vec2& localPositionStart,
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

void Body::setShapeDensity(Shape index,float density)
{
    if(body)
    {
        fixtures[index]->SetDensity(density);
        return;
    }
    shapesDef[index].fixature.density=density;
}
float Body::getShapeDensity(Shape index) const
{
    if(body)
        return fixtures.find(index)->second->GetDensity();
    return shapesDef.find(index)->second.fixature.density;
}

void Body::setShapeFriction(Shape index,float friction)
{
    if(body)
    {
        fixtures[index]->SetFriction(friction);
        return;
    }
    shapesDef[index].fixature.friction=friction;
}
float Body::getShapeFriction(Shape index) const
{
    if(body)
    {
        return fixtures.find(index)->second->GetFriction();
    }
    return shapesDef.find(index)->second.fixature.friction;
}

void Body::setShapeRestitution(Shape index,float restitution)
{
    if(body)
    {
        fixtures[index]->SetRestitution(restitution);
        return;
    }
    shapesDef[index].fixature.restitution=restitution;
}
float Body::getShapeRestitution(Shape index) const
{
    if(body)
        return fixtures.find(index)->second->GetRestitution();
    return shapesDef.find(index)->second.fixature.restitution;
}

void Body::setShapeIsSensor(Shape index,bool sensor)
{
    if(body)
    {
        fixtures[index]->SetSensor(sensor);
        return;
    }
    shapesDef[index].fixature.isSensor=sensor;
}
bool Body::getShapeIsSensor(Shape index) const
{
    if(body)
        return fixtures.find(index)->second->IsSensor();
    return shapesDef.find(index)->second.fixature.isSensor;
}


size_t Body::countShape() const
{
    if(body)
    {
        return fixtures.size();
    }
    return shapesDef.size();
}
Body::GeometryType Body::getShapeType(Shape index) const
{
    if(body)
    {
        return (GeometryType)(fixtures.find(index)->second->GetShape()->GetType());
    }
    return (GeometryType)(shapesDef.find(index)->second.type);
}
//circle
Vec2  Body::getCirclePosition(Shape index) const
{
    if(body)
    {
        b2CircleShape* shape=((b2CircleShape*)fixtures.find(index)->second->GetShape());
        return cast(shape->m_p) PIXEL_RATIO ;
    }
    return shapesDef.find(index)->second.position;
    
}
float Body::getCircleRadius(Shape index) const
{
    if(body)
    {
        b2CircleShape* shape=((b2CircleShape*)fixtures.find(index)->second->GetShape());
        return shape->m_radius PIXEL_RATIO ;
    }
    return shapesDef.find(index)->second.radius;
    
}
//polygon
void  Body::getPolygonPoints(Shape index,std::vector<Vec2>& points) const
{
    if(body)
    {
        b2PolygonShape* shape=((b2PolygonShape*)fixtures.find(index)->second->GetShape());
        points.resize(shape->m_count);
        for(uint i=0;i!=shape->m_count;++i)
        {
            points[i]=cast(shape->m_vertices[i]) PIXEL_RATIO ;
        }
    }
    else
    {
        points=shapesDef.find(index)->second.points;
    }
}
//chain
void  Body::getChainPoints(Shape index,std::vector<Vec2>& points) const
{
    if(body)
    {
        b2ChainShape* shape=((b2ChainShape*)fixtures.find(index)->second->GetShape());
        points.resize(shape->m_count);
        for(uint i=0;i!=shape->m_count;++i)
        {
            points[i]=cast(shape->m_vertices[i]) PIXEL_RATIO ;
        }
    }
    else
    {
        points=shapesDef.find(index)->second.points;
    }
}
bool  Body::getChainStartPoint(Shape index,Vec2& point) const
{
    if(body)
    {
        b2ChainShape* shape=((b2ChainShape*)fixtures.find(index)->second->GetShape());
        if(shape->m_hasPrevVertex)
        {
            point=cast(shape->m_prevVertex);
            return true;
        }
        return false;
    }
    
    if(shapesDef.find(index)->second.flags & 0x01 )
    {
        point=shapesDef.find(index)->second.prev;
        return true;
    }
    return false;
}
bool  Body::getChainEndPoint(Shape index,Vec2& point)  const
{
    if(body)
    {
        b2ChainShape* shape=((b2ChainShape*)fixtures.find(index)->second->GetShape());
        if(shape->m_hasNextVertex)
        {
            point=cast(shape->m_nextVertex);
            return true;
        }
        return false;
    }
    
    if(shapesDef.find(index)->second.flags & 0x02 )
    {
        point=shapesDef.find(index)->second.next;
        return true;
    }
    return false;
}
//edge
void  Body::getEdgePoints(Shape index,Vec2& start,Vec2& end)  const
{
    if(body)
    {
        b2EdgeShape* shape=((b2EdgeShape*)fixtures.find(index)->second->GetShape());
        start = cast(shape->m_vertex1);
        end = cast(shape->m_vertex2);
    }
    else
    {
        start=shapesDef.find(index)->second.points[0];
        end=shapesDef.find(index)->second.points[1];
    }
}
bool  Body::getEdgeStartPoint(Shape index,Vec2& point)  const
{
    if(body)
    {
        b2EdgeShape* shape=((b2EdgeShape*)fixtures.find(index)->second->GetShape());
        if(shape->m_hasVertex0)
        {
            point=cast(shape->m_vertex0);
            return true;
        }
        return false;
    }
    
    if(shapesDef.find(index)->second.flags & 0x01 )
    {
        point=shapesDef.find(index)->second.prev;
        return true;
    }
    return false;
}
bool  Body::getEdgeEndPoint(Shape index,Vec2& point)  const
{
    if(body)
    {
        b2EdgeShape* shape=((b2EdgeShape*)fixtures.find(index)->second->GetShape());
        if(shape->m_hasVertex3)
        {
            point=cast(shape->m_vertex3);
            return true;
        }
        return false;
    }
    
    if(shapesDef.find(index)->second.flags & 0x02 )
    {
        point=shapesDef.find(index)->second.next;
        return true;
    }
    return false;
}

//Geometry change
//Circle
void Body::changeCirclePosition(Shape index,const Vec2& pos)
{
    if(body)
    {
        b2CircleShape* shape=((b2CircleShape*)fixtures.find(index)->second->GetShape());
        shape->m_p=cast(pos PTM_RATIO);
    }
    else
    {
        shapesDef.find(index)->second.position=pos;
    }
}
void Body::changeCircleRadius(Shape index,float radius)
{
    if(body)
    {
        b2CircleShape* shape=((b2CircleShape*)fixtures.find(index)->second->GetShape());
        shape->m_radius= radius PTM_RATIO;
    }
    else
    {
        shapesDef.find(index)->second.radius=radius;
    }
}
//Polygon
void Body::changePolygonPoints(Shape index,const std::vector<Vec2>& argpoints)
{
    
    //debug
    DEBUG_ASSERT_MSG( argpoints.size() >= 3 && argpoints.size() <= b2_maxPolygonVertices ,
                     "changePoligonPoints error: "
                     "points numbers must to be between 3 and "+
                     String::toString(b2_maxPolygonVertices)+
                     ", you added "+
                     String::toString(argpoints.size()));
    
    if(body)
    {
        b2PolygonShape* shape=((b2PolygonShape*)fixtures.find(index)->second->GetShape());
        //update vertex
        std::vector<Vec2> points=argpoints;
        for(auto& p:points) p=p PTM_RATIO;
        //set vertexs
        shape->Set((b2Vec2*)&points[0], points.size());

    }
    else
    {
        shapesDef.find(index)->second.points=argpoints;
    }
}
//Chain
void Body::changeChainPoints(Shape index,const std::vector<Vec2>& argpoints)
{
    if(body)
    {
        b2ChainShape* shape=((b2ChainShape*)fixtures.find(index)->second->GetShape());
        //update vertex
        std::vector<Vec2> points=argpoints;
        for(auto& p:points) p=p PTM_RATIO;
        //set vertexs
        shape->~b2ChainShape(); //dealloc last vector
        shape->CreateChain((b2Vec2*)&points[0], points.size());
    }
    else
    {
        shapesDef.find(index)->second.points=argpoints;
    }
}
void Body::changeChainPoints(Shape index,const std::vector<Vec2>& argpoints,
                             bool startp,
                             const Vec2& adjacentStartPoint,
                             bool endp,
                             const Vec2& adjacentEndPoint)
{
    if(body)
    {
        b2ChainShape* shape=((b2ChainShape*)fixtures.find(index)->second->GetShape());
        //update vertex
        std::vector<Vec2> points=argpoints;
        for(auto& p:points) p=p PTM_RATIO;
        //set vertexs
        shape->~b2ChainShape();
        shape->CreateChain((b2Vec2*)&points[0], points.size());
        //change
        if(startp)
            shape->SetPrevVertex(cast(adjacentStartPoint PTM_RATIO ));
        if(startp)
            shape->SetNextVertex(cast(adjacentEndPoint PTM_RATIO ));
    }
    else
    {
        shapesDef.find(index)->second.points=argpoints;
        shapesDef.find(index)->second.flags  = 0;
        shapesDef.find(index)->second.flags  = 0x01 * startp;
        shapesDef.find(index)->second.flags |= 0x02 * endp;
        shapesDef.find(index)->second.prev   = adjacentStartPoint;
        shapesDef.find(index)->second.next   = adjacentEndPoint;
        
    }
}


////////////////////

void Body::deleteShape(Shape index)
{
    if(body)
    {
        auto itfix=fixtures.find(index);
        body->DestroyFixture(itfix->second);
        fixtures.erase(itfix);
    }
    else
    {
        shapesDef.erase(index);
    }
}

////////////////////HIDE METHODS
void Body::setAngle(Angle angle)
{
    if(body)
    {
        body->SetTransform(body->GetPosition(), angle.valueRadians());
        return;
    }
    bodyDefinition.angle= angle.valueRadians();
}
Angle Body::getAngle() const
{
    if(body)
        return Angle( Radian(body->GetAngle()) );
    return Angle( Radian(bodyDefinition.angle) );;
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
//////////////////////////////

void Body::onRun(float dt)
{
    if(getObject())
    {
        //update object
        getObject()->getGlobalMatrix();
        //create transoform
        Transform2D t2d;
        t2d.position=getPosition();
        t2d.alpha=getAngle();
        t2d.scale=getObject()->getScale();
        //set data
        getObject()->forceTransform(t2d,true);
    }
}

//object events
void Body::onSetObject(Object* object)
{
    if(object->getScene() && !world)
    {
        registerWorld((World*)getObject()->getScene());
    }
    setPosition(object->getPosition(true));
    setAngle(object->getRotation(true));
    setScale(object->getScale(true));
}
void Body::onChangedMatrix()
{
    setPosition(getObject()->getPosition(true));
    setAngle(getObject()->getRotation(true));
    setScale(getObject()->getScale(true));
}
void Body::onEraseObject()
{
    if(world)
        unregisterWorld();
}
//scene events
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
//scale
void Body::setScale(const Vec2& argScale)
{
    if(!enableScale) return;
    if(lastScale==argScale) return;
    //scale
    Vec2 scale(argScale);
	if (isAbsoluteScale) scale.abs();
    //safe scale
    SAFE_SCALE(
        if(std::abs((float)scale.x) <=  SCALE_EPSILON)
               scale.x=SCALE_EPSILON*(1.0-2.0*std::signbit(scale.x));
        if(std::abs((float)scale.y) <=  SCALE_EPSILON)
               scale.y=SCALE_EPSILON*(1.0-2.0*std::signbit(scale.y));
    );
    //max scale
    float maxLastScale=Math::max(fabs(lastScale.x),fabs(lastScale.y));
    float maxScale=Math::max(fabs(scale.x),fabs(scale.y));
    //update geometry
    if(body)
    {
        
        addScaleB2DShapes(Vec2::ONE/lastScale,1.0f/maxLastScale);
        if(scale!=Vec2::ONE) addScaleB2DShapes(scale, maxScale);
    }
    else
    {
        addScaleE2DShapes(Vec2::ONE/lastScale,1.0f/maxLastScale);
        if(scale!=Vec2::ONE) addScaleE2DShapes(scale, maxScale);
    }
    //save
    lastScale=scale;
}
//scale utilities
void Body::addScaleB2DShapes(const Vec2& scale,float radius)
{
    //for all fixture
    for( auto pFixture : fixtures )
    {
        switch (pFixture.second->GetType())
        {
            case b2Shape::e_circle:
            {
                auto* shape=(b2CircleShape*)pFixture.second->GetShape();
                
                shape->m_p.x = shape->m_p.x * scale.x ;
                shape->m_p.y = shape->m_p.y * scale.y ;
                shape->m_radius = shape->m_radius * radius;
            }
                break;
            case b2Shape::e_edge:
            {
                auto* shape=(b2EdgeShape*)pFixture.second->GetShape();
                
                shape->m_vertex0.x = shape->m_vertex0.x * scale.x ;
                shape->m_vertex0.y = shape->m_vertex0.y * scale.y ;
                
                shape->m_vertex1.x = shape->m_vertex1.x * scale.x ;
                shape->m_vertex1.y = shape->m_vertex1.y * scale.y ;
                
                shape->m_vertex2.x = shape->m_vertex2.x * scale.x ;
                shape->m_vertex2.y = shape->m_vertex2.y * scale.y ;
                
                shape->m_vertex3.x = shape->m_vertex3.x * scale.x ;
                shape->m_vertex3.y = shape->m_vertex3.y * scale.y ;
            }
                break;
                //N.b. std::vector<Vec2> points=this->points;
            case b2Shape::e_polygon:
            {
                auto* shape=(b2PolygonShape*)pFixture.second->GetShape();
                //update vertexs
                for(int i=0;i<shape->m_count;++i)
                {
                    shape->m_vertices[i].x=shape->m_vertices[i].x * scale.x;
                    shape->m_vertices[i].y=shape->m_vertices[i].y * scale.y;
                }
            }
                break;
            case b2Shape::e_chain:
            {
                auto* shape=(b2ChainShape*)pFixture.second->GetShape();
                //update vertexs
                for(int i=0;i<shape->m_count;++i)
                {
                    shape->m_vertices[i].x=shape->m_vertices[i].x * scale.x;
                    shape->m_vertices[i].y=shape->m_vertices[i].y * scale.y;
                }
                
                shape->m_prevVertex.x = shape->m_prevVertex.x * scale.x;
                shape->m_prevVertex.y = shape->m_prevVertex.y * scale.y;
                
                shape->m_nextVertex.x = shape->m_nextVertex.x * scale.x;
                shape->m_nextVertex.y = shape->m_nextVertex.y * scale.y;
            }
                break;
            case b2Shape::e_typeCount:
            default: break;
        }
    }
}
void Body::addScaleE2DShapes(const Vec2& scale,float radius)
{
    //for all fixture
    for( auto pShapes : shapesDef )
    {
        switch (pShapes.second.type)
        {
            case b2Shape::e_circle:
            {
                //position
                pShapes.second.position *= scale;
                pShapes.second.radius *= radius ;
            }
            break;
            case b2Shape::e_chain:
            case b2Shape::e_polygon:
            case b2Shape::e_edge:
            {
                for(auto& point:pShapes.second.points)
                {
                    point*= scale;
                }
                pShapes.second.prev*scale;
                pShapes.second.next*=scale;
            }
            case b2Shape::e_typeCount:
            default: break;
        }
    }
}
//scale
void Body::setEnableScale(bool scale, bool isabsolute)
{
    if(!enableScale && scale) //disable scale
    {
        setScale(Vec2::ONE);
    }
    enableScale=scale;
	isAbsoluteScale = isabsolute;
}
bool Body::getEnableScale() const
{
    return enableScale;
}
bool Body::getEnableAbsoluteScale() const
{
	return isAbsoluteScale;
}

static inline void shapeSerialize(float metersInPixel,
                                  Table& tshape,
                                  const b2Shape* bShape,
                                  bool enableScale,
                                  const Vec2& lastScale)
{
    //inv scale
    Vec2 iScale(Vec2::ONE);
    if(enableScale) iScale/=lastScale;
    //
    Table& tgeometry=tshape.createTable("geometry");
    switch (bShape->GetType())
    {	
    case b2Shape::e_circle:
    {
        b2CircleShape* shape=((b2CircleShape*)bShape);
        tgeometry.set("type","circle");
        tgeometry.set("radius", shape->m_radius * Math::min(fabs(iScale.x),fabs(iScale.y))  PIXEL_RATIO );
        tgeometry.set("position", cast(shape->m_p) * iScale PIXEL_RATIO );
    }
    break;
    case b2Shape::e_edge:
    {
        tgeometry.set("type","edge");
        b2EdgeShape* shape=((b2EdgeShape*)bShape);
        if(shape->m_hasVertex0)
            tgeometry.set("v0", cast(shape->m_vertex0) * iScale  PIXEL_RATIO );

        tgeometry.set("v1", cast(shape->m_vertex1) * iScale  PIXEL_RATIO );
        tgeometry.set("v2", cast(shape->m_vertex2) * iScale  PIXEL_RATIO );

        if(shape->m_hasVertex3)
            tgeometry.set("v3", cast(shape->m_vertex3) * iScale  PIXEL_RATIO );
    }
    break;
    case b2Shape::e_polygon:
    {
        tgeometry.set("type","polygon");
        Table& tpoints=tgeometry.createTable("points");
        b2PolygonShape* shape=((b2PolygonShape*)bShape);

        for(uint i=0;i!=shape->m_count;++i)
        {
            tpoints.set(cast(shape->m_vertices[i])  * iScale  PIXEL_RATIO );
        }
    }
    break;
    case b2Shape::e_chain:
    {
        tgeometry.set("type","chain");

        Table& tpoints=tgeometry.createTable("points");
        b2ChainShape* shape=((b2ChainShape*)bShape);
        
        if(shape->m_hasPrevVertex)
            tgeometry.set("start", cast(shape->m_prevVertex)  * iScale  PIXEL_RATIO );
        if(shape->m_hasNextVertex)
            tgeometry.set("end", cast(shape->m_nextVertex)  * iScale  PIXEL_RATIO );

        for(uint i=0;i!=shape->m_count;++i)
        {
            tpoints.set(cast(shape->m_vertices[i])  * iScale  PIXEL_RATIO );
        }
    }
    break;
    case b2Shape::e_typeCount:
    default: break; //wrong
    }
}
static inline String bytes2Hex(byte2 mask)
{
	byte2 hexs[] = 
	{
		static_cast<byte2>((mask & 0xF000) >> 12),
		static_cast<byte2>((mask & 0x0F00) >>  8),
		static_cast<byte2>((mask & 0x00F0) >>  4),
		static_cast<byte2>((mask & 0x000F))      
	};
	//out string
	String out;
	//to chars
	for (uchar i = 0; i != 4; ++i)
	{
		switch (hexs[i])
		{
		case 10: out += 'A'; break;
		case 11: out += 'B'; break;
		case 12: out += 'C'; break;
		case 13: out += 'D'; break;
		case 14: out += 'E'; break;
		case 15: out += 'F'; break;
		default: out += String::toString(hexs[i]); break;
		}
	}
	//return
	return out;
}
static inline byte2 hex2Bytes(const String& str)
{
	return (byte2)strtol(str.c_str(), NULL, 16);
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
    Table& rbody=table;
    /*
    rbody.set("position",getPosition());
    rbody.set("rotation",getAngle());
     */
    rbody.set("linearVelocity",getLinearVelocity());
    rbody.set("angularVelocity",getAngularVelocity());
    rbody.set("linearDamping",getLinearDamping());
    rbody.set("angularDamping",getAngularDamping());
    rbody.set("gravityScale",getGravityScale());
    rbody.set("allowSleep",getSleepingAllowed());
    rbody.set("awake",getAwake());
    rbody.set("fixedRotation",getFixedAngle());
    rbody.set("bullet",getBullet());
	rbody.set("active", getActive());
	if (getEnableScale() && getEnableAbsoluteScale())
		rbody.set("autoScale", Vec2::ONE);
	else
		rbody.set("autoScale", (float)getEnableScale());
    rbody.set("type",getBodyType(getType()));
    
    if(body)
    {
        Table& tshapes=rbody.createTable("shapes");
        for( auto pFixture : fixtures )
        {
            Table& tshape=tshapes.createTable();
            tshape.set("density",pFixture.second->GetDensity());
            tshape.set("friction",pFixture.second->GetFriction());
            tshape.set("restitution",pFixture.second->GetRestitution());
			tshape.set("sensor", pFixture.second->IsSensor());
			tshape.set("membershipMask", bytes2Hex(pFixture.second->GetFilterData().categoryBits));
			tshape.set("toCollideMask", bytes2Hex(pFixture.second->GetFilterData().maskBits));
			tshape.set("groupMask", bytes2Hex(pFixture.second->GetFilterData().groupIndex));
            //shape      
            shapeSerialize(metersInPixel,
                           tshape,
                           pFixture.second->GetShape(),
                           enableScale,
                           lastScale);
        }
    }
    else
    {
        //TO DO SAVE SHAPE DEF
        Table& tshapes=rbody.createTable("shapes");
        for( auto pShapeDef : shapesDef )
        {
            Table& tshape=tshapes.createTable();
            tshape.set("density",pShapeDef.second.fixature.density);
            tshape.set("friction",pShapeDef.second.fixature.friction);
            tshape.set("restitution",pShapeDef.second.fixature.restitution);
			tshape.set("sensor", pShapeDef.second.fixature.isSensor);
			tshape.set("membershipMask", bytes2Hex(pShapeDef.second.fixature.filter.categoryBits));
			tshape.set("toCollideMask", bytes2Hex(pShapeDef.second.fixature.filter.maskBits));
			tshape.set("groupMask", bytes2Hex(pShapeDef.second.fixature.filter.groupIndex));
            //shape  (not work)
            shapeSerialize(metersInPixel,
                           tshape,
                           pShapeDef.second.fixature.shape,
                           enableScale,
                           lastScale);
        }
    }
}

void Body::deserialize(const Table& table)
{
    /*
    Vec2  position(getPosition());
    float angle(getAngle());
    //from object?
    if(getObject())
    {
        if(table.getString("position","false")=="true")
            position=getObject()->getPosition(true);
        if(table.getString("rotation","false")=="true")
            angle=getObject()->getRotation(true);
    }
    setPosition( table.getVector2D("position", position) );
    setAngle( table.getFloat("rotation", angle ) );
     */
    setLinearVelocity( table.getVector2D("linearVelocity", getLinearVelocity()) );
    setAngularVelocity( table.getFloat("angularVelocity", getAngularVelocity()) );
    setLinearDamping( table.getFloat("linearDamping", getLinearDamping()) );
    setAngularDamping( table.getFloat("angularDamping", getAngularDamping()) );
    setGravityScale( table.getFloat("gravityScale",getGravityScale()) );
    setSleepingAllowed( table.getFloat("allowSleep", (float)getSleepingAllowed())!=0.0f );
    setAwake( table.getFloat("awake", (float)getAwake())!=0.0f );
    setFixedAngle( table.getFloat("fixedRotation", (float)getFixedAngle())!=0.0f );
    setBullet( table.getFloat("bullet", (float)getBullet())!=0.0f );
    setActive( table.getFloat("active", (float)getActive())!=0.0f );
	if (table.existsAsType("autoScale", Table::VECTOR2D))
	{
		Vec2 defaultValues((float)getEnableScale(), (float)getEnableAbsoluteScale());
		Vec2 values = table.getVector2D("autoScale", defaultValues);
		setEnableScale(values.x != 0.0f, values.y != 0.0f);
	}
	else
	{
		setEnableScale(table.getFloat("autoScale", (float)getEnableScale()) != 0.0f);
	}
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
                idshape=createCircleShape(
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
                idshape=createEdgeShape(v1,v2,
                                        bv0,v0,
                                        bv3,v3);
            }
            else if(type=="box")
            {
                
                DEBUG_ASSERT(geometry.existsAsType("scale",Table::VECTOR2D));
                Vec2 scale=geometry.getVector2D("scale");
                Vec2 position=geometry.getVector2D("position");
                float angle=geometry.getFloat("angle");
                idshape=createBoxShape(scale,position,angle);
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
                idshape=createPolygonShape(vp);
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

                idshape=createChainShape(vp,bstart,vstart,
                                            bend,vend);
            }
            else
            {
                DEBUG_ASSERT(0);
            }
            setShapeDensity(idshape,shape.getFloat("density",defaultFixture.density));
            setShapeFriction(idshape,shape.getFloat("friction",defaultFixture.friction));
            setShapeRestitution(idshape,shape.getFloat("restitution",defaultFixture.restitution));
            setShapeIsSensor(idshape,shape.getFloat("sensor",(float)defaultFixture.isSensor)!=0.0f);

			setShapeGroupMask(idshape, hex2Bytes(shape.getString("membershipMask", bytes2Hex(defaultFixture.filter.categoryBits))));
			setShapeGroupMask(idshape, hex2Bytes(shape.getString("toCollideMask", bytes2Hex(defaultFixture.filter.maskBits))));
			setShapeGroupMask(idshape, hex2Bytes(shape.getString("groupMask", bytes2Hex(defaultFixture.filter.groupIndex))));
        }
    }
}
