#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>
#include <Config.h>
#include <Object.h>

namespace Easy2D {
namespace Physics{

	//using namespace Box2D;
	typedef Box2D::int32 int32;
	typedef Box2D::float32 float32;
	typedef Box2D::b2DestructionListener DestructionListener;
	typedef Box2D::b2ContactFilter ContactFilter;
	typedef Box2D::b2ContactListener ContactListener;
	typedef Box2D::b2ContactListener ContactListener;
	typedef Box2D::b2Draw Draw;
	typedef Box2D::b2BodyDef BodyDef;
	typedef Box2D::b2Joint Joint;
	typedef Box2D::b2JointDef JointDef;
	typedef Box2D::b2QueryCallback QueryCallback;
	typedef	Box2D::b2RayCastCallback RayCastCallback;
	typedef Box2D::b2AABB AABB;
	typedef Box2D::b2Contact Contact;
	typedef Box2D::b2ContactManager ContactManager;
	typedef Box2D::b2Profile Profile;

	class Body;
	class World;

	/*
	body definiction
	*/
	class Body : public Object {
	
		Box2D::b2Body *body;
		Body(Box2D::b2Body *body):body(body){
			body->SetUserData(this);
		}
		friend class World;

	public:

	
	};
	
	/*
	World definiction
	*/
	class World {
		
		Box2D::b2World world;

	public:

		World (Vec2 gravity):world(gravity){}
		void setDestructionListener (DestructionListener *listener){
			world.SetDestructionListener(listener);
		}
		void setContactFilter (ContactFilter *filter){
			world.SetContactFilter(filter);
		}
		void setContactListener (ContactListener *listener){
			world.SetContactListener(listener);
		}
		void setDebugDraw (Draw *debugDraw){
			
		}
		Body* createBody (const BodyDef *def){
			return new Body(world.CreateBody(def));
		}
		void destroyBody (Body *body){
			world.DestroyBody(body->body);
			delete body;
		}
		Joint*  createJoint (const JointDef *def){
			return world.CreateJoint(def);
		}
		void destroyJoint (Joint *joint){
			 world.DestroyJoint(joint);
		}
		void step (float32 timeStep, int32 velocityIterations, int32 positionIterations){
			world.Step(timeStep,velocityIterations,positionIterations);
		}
		void clearForces (){
			world.ClearForces();
		}
		//Call this to draw shapes and other debug draw data. 
		void drawDebugData (){
			world.DrawDebugData();
		}
		void queryAABB (QueryCallback *callback, const AABB &aabb) const{
			world.QueryAABB(callback,aabb);
		}
		void rayCast (RayCastCallback *callback, Vec2 point1, Vec2 point2) const{
			world.RayCast(callback,point1,point2);
		}
		Body *getBodyList (){
			return (Body*)(world.GetBodyList()->GetUserData());
		}
		const Body *getBodyList () const{
			return (const Body*)(world.GetBodyList()->GetUserData());
		}
		Joint *getJointList (){
			return world.GetJointList();
		}
		const Joint *getJointList () const{
			return world.GetJointList();
		}
		Contact* getContactList(){
			return world.GetContactList();
		}
		const Contact *getContactList () const{
			return world.GetContactList();
		}
		//Enable/disable sleep. 
		void setAllowSleeping (bool flag){
			world.SetAllowSleeping(flag);
		}
		bool getAllowSleeping () const{
			return world.GetAllowSleeping();
		}
		//Enable/disable warm starting. For testing. 
		void setWarmStarting (bool flag){
			world.SetWarmStarting(flag);
		}
		bool getWarmStarting () const{
			return world.GetWarmStarting();
		}
		//Enable/disable continuous physics. For testing. 
		void setContinuousPhysics (bool flag){
			world.SetContinuousPhysics(flag);
		}
		bool getContinuousPhysics () const{
			return world.GetContinuousPhysics();
		}
		//Enable/disable single stepped continuous physics. For testing. 
		void setSubStepping (bool flag){
			world.SetSubStepping(flag);
		}
		bool getSubStepping () const{
			return world.GetSubStepping();
		}
		//Get the number of broad-phase proxies. 
		int32 getProxyCount () const{
			return world.GetProxyCount();
		}
		//Get the number of bodies. 
		int32 getBodyCount () const{
			return world.GetBodyCount();
		}
		//Get the number of joints. 
		int32 getJointCount () const{
			return world.GetJointCount();
		}
		//Get the number of contacts (each may have 0 or more contact points). 
		int32 getContactCount () const{
			return world.GetContactCount();
		}
		//Get the height of the dynamic tree. 
		int32 getTreeHeight () const{
			return world.GetTreeHeight();
		}
		//Get the balance of the dynamic tree. 
		int32 getTreeBalance () const{
			return world.GetTreeBalance();
		}
		float32 getTreeQuality () const{
			return world.GetTreeQuality();
		}
		//Change the global gravity vector. 
		void  setGravity (Vec2 gravity){
			world.SetGravity(gravity);
		}
		//Get the global gravity vector. 
		Vec2  getGravity () const{
			return world.GetGravity();
		}
		//Is the world locked (in the middle of a time step). 
		bool  isLocked () const{
			return world.IsLocked();
		}
		//Set flag to control automatic clearing of forces after each time step. 
		void  setAutoClearForces (bool flag){
			world.SetAutoClearForces(flag);
		}
		//Get the flag that controls automatic clearing of forces after each time step. 
		bool  getAutoClearForces () const{
			return world.GetAutoClearForces();
		}
		//Get the contact manager for testing. 
		const ContactManager &getContactManager () const{
			return world.GetContactManager();
		}
		//Get the current profile. 
		const Profile &getProfile () const{
			return world.GetProfile();
		}
		void  dump (){
			world.Dump();
		}
	};

	
};
};

#endif