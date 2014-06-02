#ifndef BODY_H
#define BODY_H

#include <Config.h>
#include <Math3D.h>
#include <Box2D.hpp>

namespace Easy2D {
	/////////////////////
    class World;
    class Object;
    typedef uint Shape;
	/////////////////////
	class Body {
        
        //friend class
        friend class World;
        friend class Object;
        
        //objects
        World*  world;
        b2Body* body;
        b2BodyDef bodyDefinition;
        float mass; //not in bodyDefinition
        
        //word
        void registerWorld(World *world);
        void unregisterWorld(World *world);
        
        //list fixture
        b2FixtureDef defaultFixture;
        std::vector<b2Fixture*> fixtures;
        std::vector<b2FixtureDef*> fixturesDef;
        
    protected:

        Body();
        virtual ~Body();
        b2Body* getBody(){ return body; }
        void getTransform(Transform2D& t2d) const;
        
	public:

		enum Type{
			DINAMIC=b2BodyType::b2_dynamicBody,
			KINEMATIC=b2BodyType::b2_kinematicBody,
			STATIC=b2BodyType::b2_staticBody
		};

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
		Shape createEdgeCollisionShape( const Vec2& localPositionStart,
										 const Vec2& localPositionEnd, 
										 const Vec2& adjacentStartPoint, 
										 const Vec2&  adjacentEndPoint );
		
		void setCollisionShapeDensity(Shape index,float density);
		float getCollisionShapeDensity(Shape index);

		void setCollisionShapeFriction(Shape index,float friction);
		float getCollisionShapeFriction(Shape index) const;
		
		void setCollisionShapeRestitution(Shape index,float restitution);
		float getCollisionShapeRestitution(Shape index) const;
		
		void setCollisionShapeIsSensor(Shape index,bool isSensor);
		bool getCollisionShapeIsSensor(Shape index) const;


	};

};

#endif