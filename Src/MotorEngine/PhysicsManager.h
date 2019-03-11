#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "Game.h"

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

	//Deberían ser métodos básicos del los objetos, ya que no necesitan interaccion desde el physics manager
	void Move(btRigidBody* rb,SceneNode* node, btVector3 dir, float speed);
	void Rotate(btRigidBody * rb, SceneNode * node, btQuaternion rot, float speed);

	void AddDiscreteImpulse(btVector3 dir, btScalar strenght, int group);
	int AddRigidBodyToGroup(btRigidBody* rb, int group);

	btRigidBody* CreateBoxCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 size);
	btRigidBody* CreateSphereCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar radius);
	btRigidBody* CreateCapsuleCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar height, btScalar radius);
	btRigidBody* CreatePlaneCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 normalDir, btScalar size);
	
	

private:
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	//Método que completa el objeto físico
	btRigidBody* CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor);

	//Tenia pensado tener vectores de rigidBodies para poder elegir sobre que grupo de rigidbody generar fuerzas o actuar, podria haber
	//el vector de elementos estaticos
	//el vector de enemigos
	//el vector con el personaje
	//el vector con ...	en función del juego
	std::vector<std::vector<btRigidBody*>> rigidBodiesGroups;
};

