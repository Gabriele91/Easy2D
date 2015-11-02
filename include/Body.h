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
    
    
    //list fixture (word)
    b2FixtureDef defaultFixture;
    DUNORDERED_MAP< Shape, b2Fixture* > fixtures;
    typedef DUNORDERED_MAP< Shape, b2Fixture* >::iterator ITFixtures;
    typedef DUNORDERED_MAP< Shape, b2Fixture* >::const_iterator CITFixtures;
    //save boxy (no word)
    struct ShapeDef
    {
        //type
        b2Shape::Type type;
        //fixature def
        b2FixtureDef  fixature;
        //info sphere
        Vec2 position;
        float radius;
        uchar flags;
        Vec2  prev,next;
        //info other
        std::vector<Vec2> points;
        //build shape
        void buildShape(float metersUnit);
    };
    //shape def
    DUNORDERED_MAP< Shape, ShapeDef > shapesDef;
    typedef DUNORDERED_MAP< Shape, ShapeDef >::iterator ITShapeDef;
    typedef DUNORDERED_MAP< Shape, ShapeDef >::const_iterator CITShapeDef;
    
    public:

    struct Impulse
    {
        uchar npoints;
        float normalImpulse[2];
        float tangentImpulse[2];
        //info from b2ContactImpulse
		void setContactImpulse(const b2ContactImpulse& im);
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
		void setManifold(const b2Manifold& m);
    };

    struct Info
    {
        Shape shapeA;
        Shape shapeB;
        b2Contact* contact;
        Manifold manifold;
        uchar orderCase;
    };

    //ShapeIteratos
    class ShapeIterator
    {
        //////////////////
        friend class Body;
        //////////////////
        
        bool isFix;
        ITFixtures itFix;
        ITShapeDef itShape;
        
		ShapeIterator(ITFixtures itFix);
		ShapeIterator(ITShapeDef itShape);
        
    public:
        
		ShapeIterator operator++();
		Shape operator*() const;        
		bool operator==(const ShapeIterator& rhs) const;        
		bool operator!=(const ShapeIterator& rhs) const;
        
    };
    //ShapeIteratos
    class CShapeIterator
    {
        //////////////////
        friend class Body;
        //////////////////
        
        bool isFix;
        CITFixtures itFix;
        CITShapeDef itShape;
        
		CShapeIterator(CITFixtures itFix);        
		CShapeIterator(CITShapeDef itShape);

    public:
        
		CShapeIterator operator++();
		const Shape operator*() const;
		bool operator==(const CShapeIterator& rhs) const;
		bool operator!=(const CShapeIterator& rhs) const;
        
    };

    
    private:
    
    //objects
    World*  world;
    b2Body* body;
    b2BodyDef bodyDefinition;
    float mass; //not in bodyDefinition
    float metersUnit; 
    float metersInPixel;

    //word
    void updatePixelScale(float metersUnit,float mPixel);
    void registerWorld(World *world);
    void unregisterWorld();
    //fromr b2Fixture* to shapedef
    void  pushShape(Shape index,b2FixtureDef* fixature, b2Shape* shape);
    //create shape in shapedef or b2Fixture*
    Shape addFixatureShape(b2FixtureDef* fixature,b2Shape* shape);
    //counter id shape
    Shape idShapeGen;
    //create id
    Shape newShapeId()
    {
        return idShapeGen++;
    }
    void resetShapeId()
    {
        idShapeGen=0;
    }
    
    protected:

    //collison call back
    DFUNCTION<void (Object* tocollide,const Info& info)> cbBegin;
    DFUNCTION<void (Object* tocollide,const Info& info)> cbEnd;
    DFUNCTION<void (Object* tocollide,const Info& info,const Manifold& oldmf)> cbPreSolver;
    DFUNCTION<void (Object* tocollide,const Info& info,const Impulse& impulse)> cbPostSolver;
    
    //return raw body
    b2Body* getBody()
    {
        return body;
    }
    //return raw body (const)
    const b2Body* getBody() const
    {
        return body;
    }
    //scale utility
    bool enableScale;
	bool isAbsoluteScale;
    Vec2 lastScale;
    //scale utilities
    void setScale(const Vec2& scale);
    void addScaleB2DShapes(const Vec2& scale,float radius);
    void addScaleE2DShapes(const Vec2& scale,float radius);
    
    
    
    public:
    
    enum Type
    {
        DINAMIC=b2BodyType::b2_dynamicBody,
        KINEMATIC=b2BodyType::b2_kinematicBody,
        STATIC=b2BodyType::b2_staticBody
    };
    
    enum GeometryType
    {
        CIRCLE=b2Shape::e_circle,
        CHAIN=b2Shape::e_chain,
        POLYGON=b2Shape::e_polygon,
        EDGE=b2Shape::e_edge,
        NEXTGT=b2Shape::e_typeCount,
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
    bool getBullet() const;
    //change b2Body angle
    void setAngle(Angle);
    //get b2Body angle
    Angle getAngle() const;
    //set b2Body posizion
    void setPosition(const Vec2& pos);
    //get b2Body posizion
    Vec2 getPosition() const;
    //scale
	void setEnableScale(bool enable, bool isabsolute = false);
	bool getEnableScale() const;
	bool getEnableAbsoluteScale() const;
    //void setMass(float); in fixtures
    float getMass() const;
    //get world
    Vec2 getWorldCenter() const;
    Vec2 getWorldPoint(const Vec2& local) const;
    //get local
    Vec2 getLocalVector(const Vec2& worldVector) const;
    Vec2 getLocalPoint(const Vec2& worldPoint) const;
    Vec2 getLocalCenter() const;
   
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

    void setFixedAngle(bool);
    bool getFixedAngle() const;

    void setGravityScale(float);
    float getGravityScale() const;

    void setSleepingAllowed(bool);
    bool getSleepingAllowed() const;
    /**
    * applay
    */
    void applyForce(const Vec2& force, const Vec2& point, bool wake);
    void applyForceToCenter(const Vec2& force, bool wake);
    void applyTorque(float torque, bool wake);
    void applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake);
	void applyAngularImpulse(float impulse, bool wake);

    /*
    * Shapes
    */
    Shape createCircleShape(float radius, const Vec2& pos=Vec2::ZERO);
    Shape createBoxShape(const Vec2& size, const Vec2& pos=Vec2::ZERO, float angle=0.0);
    Shape createPolygonShape(const std::vector<Vec2>& points);
    Shape createChainShape( const std::vector<Vec2>& points );
    Shape createChainShape( const std::vector<Vec2>& points,
                            bool startp ,
                            const Vec2& adjacentStartPoint,
                            bool endp,
                            const Vec2& adjacentEndPoint );
    Shape createEdgeShape( const Vec2& localPositionStart,
                           const Vec2& localPositionEnd);
    Shape createEdgeShape( const Vec2& localPositionStart,
                           const Vec2& localPositionEnd,
                           const bool hasAdjacentLocalPositionStart,
                           const Vec2& adjacentLocalPositionStart,
                           const bool hasAdjacentLocalPositionEnd,
                           const Vec2& adjacentLocalPositionEnd );

    void setShapeDensity(Shape index,float density);
    float getShapeDensity(Shape index) const;

    void setShapeFriction(Shape index,float friction);
    float getShapeFriction(Shape index) const;

    void setShapeRestitution(Shape index,float restitution);
    float getShapeRestitution(Shape index) const;

    void setShapeIsSensor(Shape index,bool isSensor);
    bool getShapeIsSensor(Shape index) const;
	/*
	* Groups
	*/
	void setShapeMembershipMask(Shape index, byte2 groupMask);
	byte2 getShapeMembershipMask(Shape index) const;

	void setShapeToCollideMask(Shape index, byte2 groupMask);
	byte2 getShapeToCollideMask(Shape index) const;

	void setShapeGroupMask(Shape index, byte2 layerMask);
	byte2 getShapeGroupMask(Shape index) const;
    /**
    * Shape geometry info
    */
    size_t       countShape() const;
    GeometryType getShapeType(Shape index) const;
    //it
	ShapeIterator begin();
	ShapeIterator end();
	CShapeIterator begin() const;
	CShapeIterator cbegin() const;
	CShapeIterator end() const;
	CShapeIterator cend() const;
    //circle
    Vec2  getCirclePosition(Shape index) const;
    float getCircleRadius(Shape index) const;
    //polygon
    void  getPolygonPoints(Shape index,std::vector<Vec2>& points) const;
    //chain
    void  getChainPoints(Shape index,std::vector<Vec2>& points) const;
    bool  getChainStartPoint(Shape index,Vec2& point) const;
    bool  getChainEndPoint(Shape index,Vec2& point) const;
    //edge
    void  getEdgePoints(Shape index,Vec2& start,Vec2& end) const;
    bool  getEdgeStartPoint(Shape index,Vec2& point) const;
    bool  getEdgeEndPoint(Shape index,Vec2& point) const;
    
    //Geometry change
    //Circle
    void changeCirclePosition(Shape index,const Vec2& pos);
    void changeCircleRadius(Shape index,float r);
    //Polygon
    void changePolygonPoints(Shape index,const std::vector<Vec2>& points);
    //Chain
    void changeChainPoints(Shape index,const std::vector<Vec2>& points);
    void changeChainPoints(Shape index,const std::vector<Vec2>& points,
                           bool startp,
                           const Vec2& adjacentStartPoint,
                           bool endp,
                           const Vec2& adjacentEndPoint);
    
    
    //delete a shape
    void deleteShape(Shape index);


    
    //component name
    ADD_COMPONENT_METHOS(Body)
    //run
    virtual void onRun(float dt) ;
    //scene
    virtual void onSetScene(Scene* scene);
    virtual void onEraseScene();
    //object
    virtual void onSetObject(Object* object);
    virtual void onChangedMatrix();
    virtual void onEraseObject();
    //serialize/deserialize
    virtual void serialize(Table& table);
    virtual void deserialize(const Table& table);
};
REGISTERED_COMPONENT(Body,"Body")

};

#endif