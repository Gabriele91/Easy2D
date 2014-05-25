#ifndef BODY_H
#define BODY_H

#include <Config.h>
#include <Math3D.h>

namespace Easy2D {
	

	class Body {

	public:

		enum Type{
			DINAMIC,
			KINEMATIC,
			STATIC
		};

		Body();
		/*
		* body
		*/
		void setType(Type);
		Type getType() const;

		void setActive(bool);
		bool getActive() const;
		
		void setAwake(bool);
		bool getAwake();

		void setBullet(bool);
		bool getBullet();

		void setMass(float);
		float getMass() const;
		
		void setDensity(float);
		float getDensity() const;

		void setFriction(float);
		float getFriction() const;
				
		void setLinearVelocity(const Vec2&); 
		Vec2 getLinearVelocity() const;
		Vec2 getLinearVelocityFromWorldPoint(const Vec2& world) const;
		Vec2 getLinearVelocityFromLocalPoint(const Vec2& local) const;
		
		void setAngularVelocity(float); 
		float getAngularVelocity() const;

		void setLinearDamping(float); 
		float getLinearDamping() const;
		
		void setAngularDamping(float);
		float getAngularDamping() const;
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
		ushort createCircleCollisionShape(float radius,
										  const Vec2& pos=Vec2::ZERO);
		ushort createPolygonCollisionShape(const std::vector<Vec2>& points);
		ushort createBoxCollisionShape(const Vec2& size,
									   const Vec2& pos=Vec2::ZERO,
									   float angle=0.0);
		
		ushort createChainCollisionShape( const std::vector<Vec2>& points );

		ushort createChainCollisionShape( const std::vector<Vec2>& points, 
										  const Vec2& adjacentStartPoint, 
										  const Vec2& adjacentEndPoint );
		
		ushort createEdgeCollisionShape( const Vec2& localPositionStart, 
										 const Vec2& localPositionEnd);

		ushort createEdgeCollisionShape( const Vec2& localPositionStart, 
										 const Vec2& localPositionEnd, 
										 const Vec2& adjacentStartPoint, 
										 const Vec2&  adjacentEndPoint );
		
		void setCollisionShapeDensity(ushort index,float density);
		float getCollisionShapeDensity(ushort index);

		void setCollisionShapeFriction(ushort index,float friction);
		float getCollisionShapeFriction(ushort index) const;
		
		void setCollisionShapeRestitution(ushort index,float restitution);
		float getCollisionShapeRestitution(ushort index) const;
		
		void setCollisionShapeIsSensor(ushort index,bool isSensor);
		bool getCollisionShapeIsSensor(ushort index) const;


	};

};

#endif