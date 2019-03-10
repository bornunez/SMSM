#pragma once
#include <btBulletDynamicsCommon.h>

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();	
	void Update();
	void addRigidBody(btRigidBody* rb);
	void addCollisionShape(btCollisionShape* sh);

	//GETS
	int getCollisionObjectCount() { return collisionShapes.size(); }
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }
	void addForce(float f);

private:
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

