#ifndef BODY_H
#define BODY_H

#include <Config.h>
#include <Math3D.h>
#include <Component.h>
#include <Box2D.hpp>

namespace Easy2D
{
/////////////////////
class World;
class Object;
class ContactListener;
/////////////////////
typedef uint Shape;
/////////////////////
class Body : public Component
{

    //friend class
    friend class Object;
    friend class World;
    friend class ContactListener;
    
    public:

    struct Impulse
    {
        uchar npoints;
        float normalImpulse[2];
        float tangentImpulse[2];
        //info from b2ContactImpulse
        void setContactImpulse(const b2ContactImpulse& im)
        {
            npoints=im.count;
            for(uchar i=0;i!=npoints;++i)
            {
                normalImpulse[i]=im.normalImpulses[i];
                tangentImpulse[i]=im.tangentImpulses[i];
            }
        }
    };

    struct Manifold
    {
        //contact info
        uchar npoints;
        Vec2 normal;
        Vec2 points[2];
        float normalImpulse[2];
        float tangentImpulse[2];
        //info from b2Manifold
        void setManifold(const b2Manifold& m)
        {
            //normal
            normal=cast(m.localNormal);
            //points
            npoints=m.pointCount;
            for(uchar i=0;i!=npoints;++i)
            {
                points[i]=cast(m.points[i].localPoint);
                normalImpulse[i]=m.points[i].normalImpulse;
                tangentImpulse[i]=m.points[i].tangentImpulse;
            }
        }
    };

    struct Info
    {
        Shape shapeA;
        Shape shapeB;
        b2Contact* contact;
        Manifold manifold;
    };

    private:
    //objects
    World*  world;
    b2Body* body;
    b2BodyDef bodyDefinition;
    float mass; //not in bodyDefinition

    //word
    void registerWorld(World *world);
    void unregisterWorld();

    //list fixture
    b2FixtureDef defaultFixture;
    std::vector<b2Fixture*> fixtures;
    std::vector<b2FixtureDef*> fixturesDef;
    
    protected:

    //collison call back
    DFUNCTION<void (Object* tocollide,const Info& info)> cbBegin;
    DFUNCTION<void (Object* tocollide,const Info& info)> cbEnd;
    DFUNCTION<void (Object* tocollide,const Info& info,const Manifold& oldmf)> cbPreSolver;
    DFUNCTION<void (Object* tocollide,const Info& info,const Impulse& impulse)> cbPostSolver;
    

    public:
        
    b2Body* getBody()
    {
        return body;
    }
    const b2Body* getBody() const
    {
        return body;
    }

    enum Type
    {
        DINAMIC=b2BodyType::b2_dynamicBody,
        KINEMATIC=b2BodyType::b2_kinematicBody,
        STATIC=b2BodyType::b2_staticBody
    };
    
    Body();
    virtual ~Body();
    /*
    * Listener
    */
    void setBeginListener(DFUNCTION<void (Object* tocollide,const Info& info)> cbBegin)
    {
        this->cbBegin=cbBegin;
    }
    void setEndListener(DFUNCTION<void (Object* tocollide,const Info& info)> cbEnd)
    {
        this->cbEnd=cbEnd;
    }
    void setPreSolverListener(DFUNCTION<void (Object* tocollide,const Info& info,const Manifold& oldmf)> cbPreSolver)
    {
        this->cbPreSolver=cbPreSolver;
    }
    void setPostSolverListener(DFUNCTION<void (Object* tocollide,const Info& info,const Impulse& impulse)> cbPostSolver)
    {
        this->cbPostSolver=cbPostSolver;
    }
    /*
    * body
    */
    void setType(Type);
    Type getType() const;

    void setActive(bool);
    bool getActive() const;

    void setAwake(bool);
    bool getAwake() const;

    void setBullet(bool);
    bool getBullet();

    //void setMass(float); in fixtures
    float getMass() const;
    //get world
    Vec2 getWorldCenter() const
    {
        if(body)
            return cast(body->GetWorldCenter());
        return Vec2::ZERO;
    }
    Vec2 getWorldPoint(const Vec2& local) const
    {
        if(body)
            return cast(body->GetWorldPoint(cast(local)));
        return Vec2::ZERO;
    }
    //get local
    Vec2 getLocalVector(const Vec2& worldVector) const
    {
        if(body)
            return cast(body->GetLocalVector(cast(worldVector)));
        return Vec2::ZERO;
    }
    Vec2 getLocalPoint(const Vec2& worldPoint) const
    {
        if(body)
            return cast(body->GetLocalPoint(cast(worldPoint)));
        return Vec2::ZERO;
    }
    Vec2 getLocalCenter() const
    {
        if(body)
            return cast(body->GetLocalCenter());
        return Vec2::ZERO;
    }
   
    //velocity
    void setLinearVelocity(const Vec2&);
    Vec2 getLinearVelocity() const;
    Vec2 getLinearVelocityFromWorldPoint(const Vec2& world) const;
    Vec2 getLinearVelocityFromLocalPoint(const Vec2& local) const;

    void setLinearDamping(float);
    float getLinearDamping() const;

    void setAngularVelocity(float);
    float getAngularVelocity() const;

    void setAngularDamping(float);
    float getAngularDamping() const;

    void setAngle(float);
    float getAngle() const;

    void setPosition(const Vec2& pos);
    Vec2 getPosition() const;

    void setFixedAngle(bool);
    bool getFixedAngle() const;

    void setGravityScale(float);
    float getGravityScale() const;

    void setSleepingAllowed(bool);
    bool getSleepingAllowed() const;

    /**
    * applay
    */
    void applyForce(const Vec2& force, const Vec2& point, bool wake)
    {
        if(body) body->ApplyForce(cast(force),cast(point),wake);
    }
    void applyForceToCenter(const Vec2& force, bool wake)
    {
        if(body) body->ApplyForceToCenter(cast(force),wake);
    }
    void applyTorque(float torque, bool wake)
    {
        if(body) body->ApplyTorque(torque,wake);
    }
    void applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake)
    {
        if(body) body->ApplyLinearImpulse(cast(impulse),cast(point),wake);
    }
	void applyAngularImpulse(float impulse, bool wake)
    {
        if(body) body->ApplyAngularImpulse(impulse,wake);
    }
    /*
    * Groups
    */
    void setCollisionGroupMask( byte16 groupMask );
    byte16 getCollisionGroupMask() const;

    void setCollisionLayerMask( byte16 layerMask );
    byte16 getCollisionLayerMask() const;
    /*
    * Shapes
    */
    Shape createCircleCollisionShape(float radius, const Vec2& pos=Vec2::ZERO);
    Shape createBoxCollisionShape(const Vec2& size, const Vec2& pos=Vec2::ZERO, float angle=0.0);
    Shape createPolygonCollisionShape(const std::vector<Vec2>& points);
    Shape createChainCollisionShape( const std::vector<Vec2>& points );
    Shape createChainCollisionShape( const std::vector<Vec2>& points, bool startp ,
                                     const Vec2& adjacentStartPoint,
                                     bool endp,
                                     const Vec2& adjacentEndPoint );
    Shape createEdgeCollisionShape( const Vec2& localPositionStart,
                                    const Vec2& localPositionEnd);
    Shape createEdgeCollisionShape(const Vec2& localPositionStart,
                                   const Vec2& localPositionEnd,
                                   const bool hasAdjacentLocalPositionStart,
                                   const Vec2& adjacentLocalPositionStart,
                                   const bool hasAdjacentLocalPositionEnd,
                                   const Vec2& adjacentLocalPositionEnd );

    void setCollisionShapeDensity(Shape index,float density);
    float getCollisionShapeDensity(Shape index);

    void setCollisionShapeFriction(Shape index,float friction);
    float getCollisionShapeFriction(Shape index) const;

    void setCollisionShapeRestitution(Shape index,float restitution);
    float getCollisionShapeRestitution(Shape index) const;

    void setCollisionShapeIsSensor(Shape index,bool isSensor);
    bool getCollisionShapeIsSensor(Shape index) const;

    //component name
    virtual const char* getComponentName() const
    {
        return "Body";
    }    
    static const  type_info* getComponentType()
    {
        return &typeid(Body);
    }
    //run
    virtual void onRun(float dt) ;
    //scene
    virtual void onSetScene(Scene* scene);
    virtual void onEraseScene();
    //object
    virtual void onSetObject(Object* object);
    virtual void onEraseObject();
};

};

#endif