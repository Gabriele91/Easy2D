#include <stdafx.h>
#include <Body.h>
#include <World.h>
#include <Debug.h>
#include <Box2D.hpp>
///////////////////////
using namespace Easy2D;
///////////////////////
//init
Body::Body(){
    world=NULL;
    body=NULL;
    mass = 0.0;
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
Body::~Body(){
    if(body){
        // Destroy the physics body.
        world->world->DestroyBody( body );
        //to null
        body=NULL;
    }
    //delete save fixatures
    for( auto pFixtureDef : fixturesDef ){
        // Destroy fixture shape.
        delete pFixtureDef->shape;
        // Destroy fixture definition.
        delete pFixtureDef;
    }
}
void Body::getTransform(Transform2D& t2d) const{
    t2d.position=getPosition();
    t2d.alpha=getAngle();
    
}
//word
void Body::registerWorld(World *wrd){
    //save
    world=wrd;
    //create body
    body=wrd->world->CreateBody(&bodyDefinition);
    //copy old fixatures
    for( auto pFixtureDef : fixturesDef )
    {
        // Create fixture.
        b2Fixture* pFixture = body->CreateFixture( pFixtureDef );
        
        // Push fixture.
        fixtures.push_back( pFixture );
        
        // Destroy fixture shape.
        delete pFixtureDef->shape;
        
        // Destroy fixture definition.
        delete pFixtureDef;
    }
    fixturesDef.clear();
}
void Body::unregisterWorld(World *wrd){
    // Transfer physics body configuration back to definition.
    world = NULL;
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
    //Copy fixtures to fixture definitions.
    for( auto pFixture : fixtures )
    {
        // Create fixture definition.
        b2FixtureDef* pFixtureDef = new b2FixtureDef();
        pFixtureDef->density      = pFixture->GetDensity();
        pFixtureDef->friction     = pFixture->GetFriction();
        pFixtureDef->restitution  = pFixture->GetRestitution();
        pFixtureDef->isSensor     = pFixture->IsSensor();
        pFixtureDef->userData     = this;
        pFixtureDef->shape        = pFixture->GetShape()->Clone(&wrd->blockAllocator);
        // Push fixture definition.
        fixturesDef.push_back( pFixtureDef );
    }
    fixtures.clear();
    // Destroy the physics body.
    wrd->world->DestroyBody( body );
    //to null
    body=NULL;
}


void Body::setActive(bool active){
    if(body){
        body->SetActive(active);
        return;
    }
    bodyDefinition.active=active;
}
bool Body::getActive() const{
    if(body)
        return body->IsActive();
    return bodyDefinition.active;
}

void Body::setAwake(bool awake){
    if(body){
        body->SetAwake(awake);
        return;
    }
    bodyDefinition.awake=awake;
}
bool Body::getAwake() const{
    if(body)
        return body->IsAwake();
    return bodyDefinition.awake;
}

void Body::setBullet(bool bullet){
    if(body){
        body->SetBullet(bullet);
        return;
    }
    bodyDefinition.bullet=bullet;
}
bool Body::getBullet(){
    if(body)
        return body->IsBullet();
    return bodyDefinition.bullet;
}

void Body::setType(Type type){
    if(body){
        body->SetType((b2BodyType)type);
        return;
    }
    bodyDefinition.type=(b2BodyType)type;
}
Body::Type Body::getType() const{
    if(body)
        return (Body::Type)body->GetType();
    return (Body::Type)bodyDefinition.type;
}

float Body::getMass() const{
    if(body)
        return body->GetMass();
    return mass;
}


void Body::setLinearVelocity(const Vec2& vl){
    if(body){
        body->SetLinearVelocity(cast(vl));
        return;
    }
    bodyDefinition.linearVelocity=cast(vl);
}
Vec2 Body::getLinearVelocity() const{
    if(body)
        return cast(body->GetLinearVelocity());
    return cast(bodyDefinition.linearVelocity);
}

Vec2 Body::getLinearVelocityFromWorldPoint(const Vec2& world) const{
    if(body)
        return cast(body->GetLinearVelocityFromWorldPoint(cast(world)));
    return cast(bodyDefinition.linearVelocity);
}
Vec2 Body::getLinearVelocityFromLocalPoint(const Vec2& world) const{
    if(body)
        return cast(body->GetLinearVelocityFromLocalPoint(cast(world)));
    return cast(bodyDefinition.linearVelocity);
}

void Body::setLinearDamping(float linearDamping){
    if(body){
        body->SetLinearDamping(linearDamping);
        return;
    }
    bodyDefinition.linearDamping=linearDamping;
}
float Body::getLinearDamping() const{
    if(body)
        return body->GetLinearDamping();
    return bodyDefinition.linearDamping;
}

void Body::setAngularVelocity(float angularVelocity){
    if(body){
        body->SetAngularVelocity(angularVelocity);
        return;
    }
    bodyDefinition.angularVelocity=angularVelocity;
}
float Body::getAngularVelocity() const{
    if(body)
        return body->GetAngularVelocity();
    return bodyDefinition.angularVelocity;
}

void Body::setAngularDamping(float angularDamping){
    
    if(body){
        body->SetAngularDamping(angularDamping);
        return;
    }
    bodyDefinition.angularDamping=angularDamping;
}
float Body::getAngularDamping() const{
    if(body)
        return body->GetAngularDamping();
    return bodyDefinition.angularDamping;
}


void Body::setAngle(float angle){
    if(body){
        body->SetTransform(body->GetPosition(), Math::torad(angle));
        return;
    }
    bodyDefinition.angle=Math::torad(angle);
}
float Body::getAngle() const{
    if(body)
        return Math::todeg(body->GetAngle());
    return Math::todeg(bodyDefinition.angle);
}

void Body::setPosition(const Vec2& pos){
    if(body){
        body->SetTransform(cast(pos), body->GetAngle());
        return;
    }
    bodyDefinition.position=cast(pos);
}
Vec2 Body::getPosition() const{
    if(body)
        return cast(body->GetPosition());
    return cast(bodyDefinition.position);
}

void Body::setFixedAngle(bool fixed){
    if(body){
        body->SetFixedRotation(fixed);
        return;
    }
    bodyDefinition.fixedRotation=fixed;
}
bool Body::getFixedAngle() const{
    if(body)
        return body->IsFixedRotation();
    return bodyDefinition.fixedRotation;
}


void Body::setGravityScale(float gravityScale){
    if(body){
        body->SetGravityScale(gravityScale);
        return;
    }
    bodyDefinition.gravityScale=gravityScale;
}
float Body::getGravityScale() const{
    if(body)
        return body->GetGravityScale();
    return bodyDefinition.gravityScale;
}

void Body::setSleepingAllowed(bool sleepingAllowed){
    if(body){
        body->SetSleepingAllowed(sleepingAllowed);
        return;
    }
    bodyDefinition.allowSleep=sleepingAllowed;
}
bool Body::getSleepingAllowed() const{
    if(body)
        return body->IsSleepingAllowed();
    return bodyDefinition.allowSleep;
}


Shape Body::createCircleCollisionShape(float radius, const Vec2& pos){
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2CircleShape* pShape = new b2CircleShape();
    pShape->m_p = cast(pos);
    pShape->m_radius = radius;
    pFixtureDef->shape = pShape;
    //
    if ( body )
    {
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        
        //return id
        return fixtures.size()-1;
    }
    // Push fixture definition.
    fixturesDef.push_back( pFixtureDef );
    //return id
    return fixturesDef.size()-1;
}
Shape Body::createBoxCollisionShape(const Vec2& size, const Vec2& pos, float angle){
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2PolygonShape* pShape = new b2PolygonShape();
    pShape->SetAsBox(size.x, size.y, cast(pos), angle);
    pFixtureDef->shape = pShape;
    //
    if ( body )
    {
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        
        //return id
        return fixtures.size()-1;
    }
    // Push fixture definition.
    fixturesDef.push_back( pFixtureDef );
    //return id
    return fixturesDef.size()-1;
}


Shape Body::createPolygonCollisionShape( const std::vector<Vec2>& points ){
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2PolygonShape* pShape = new b2PolygonShape();
    
    DEBUG_ASSERT_MSG( points.size() >= 3 && points.size() <= b2_maxPolygonVertices ,
                     "createChainCollisionShape error: "
                     "points numbers must to be between 3 and "+
                     String::toString(b2_maxPolygonVertices)+
                     ", you added "+
                     String::toString(points.size()));
    
    
    pShape->Set((b2Vec2*)&points[0], points.size());
    pFixtureDef->shape = pShape;
    
    if ( body )
    {
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        
        //return id
        return fixtures.size()-1;
    }
    // Push fixture definition.
    fixturesDef.push_back( pFixtureDef );
    //return id
    return fixturesDef.size()-1;
    
}

Shape Body::createChainCollisionShape( const std::vector<Vec2>& points ){
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2ChainShape* pShape = new b2ChainShape();

    pShape->CreateChain((b2Vec2*)&points[0], points.size());
    pFixtureDef->shape = pShape;
    
    if ( body )
    {
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        
        //return id
        return fixtures.size()-1;
    }
    // Push fixture definition.
    fixturesDef.push_back( pFixtureDef );
    //return id
    return fixturesDef.size()-1;
    
}

Shape Body::createChainCollisionShape( const std::vector<Vec2>& points,
                                       bool startp ,
                                       const Vec2& adjacentStartPoint,
                                       bool endp,
                                       const Vec2& adjacentEndPoint ){
    // Configure fixture definition.
    b2FixtureDef* pFixtureDef = new b2FixtureDef( defaultFixture );
    b2ChainShape* pShape = new b2ChainShape();
    
    pShape->CreateChain((b2Vec2*)&points[0], points.size());
    pFixtureDef->shape = pShape;
    
    if(startp)
        pShape->SetPrevVertex(cast(adjacentStartPoint));
    if(endp)
        pShape->SetNextVertex(cast(adjacentEndPoint));
    
    if ( body )
    {
        // Create and push fixture.
        fixtures.push_back( body->CreateFixture( pFixtureDef ) );
        
        // Destroy shape and fixture.
        delete pShape;
        delete pFixtureDef;
        
        //return id
        return fixtures.size()-1;
    }
    // Push fixture definition.
    fixturesDef.push_back( pFixtureDef );
    //return id
    return fixturesDef.size()-1;
    
}

void Body::setCollisionShapeDensity(Shape index,float density){
    if(body){
        fixtures[index]->SetDensity(density);
        return;
    }
    fixturesDef[index]->density=density;
}
float Body::getCollisionShapeDensity(Shape index){
    if(body)
        return fixtures[index]->GetDensity();
    return fixturesDef[index]->density;
}

void Body::setCollisionShapeFriction(Shape index,float friction){
    if(body){
        fixtures[index]->SetFriction(friction);
        return;
    }
    fixturesDef[index]->friction=friction;
}
float Body::getCollisionShapeFriction(Shape index) const{
    if(body)
        return fixtures[index]->GetFriction();
    return fixturesDef[index]->friction;
}

void Body::setCollisionShapeRestitution(Shape index,float restitution){
    if(body){
        fixtures[index]->SetRestitution(restitution);
        return;
    }
    fixturesDef[index]->restitution=restitution;
}
float Body::getCollisionShapeRestitution(Shape index) const{
    if(body)
        return fixtures[index]->GetRestitution();
    return fixturesDef[index]->restitution;
}

void Body::setCollisionShapeIsSensor(Shape index,bool sensor){
    if(body){
        fixtures[index]->SetSensor(sensor);
        return;
    }
    fixturesDef[index]->isSensor=sensor;
}
bool Body::getCollisionShapeIsSensor(Shape index) const{
    if(body)
        return fixtures[index]->IsSensor();
    return fixturesDef[index]->isSensor;
}
