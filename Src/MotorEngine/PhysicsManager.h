#pragma once
#include <btBulletDynamicsCommon.h>
#include "Game.h"
#include "RigidBodyComponent.h"

struct bulletObject {
	
	RigidBodyComponent* _rb;	// Permitirá llamar al collisionHandler del objeto al colisionar
	Ogre::SceneNode* _node;		// Servirá para detectar nuestro bullet object
	int _id;					// Servirá para saber el tipo de objeto que tenemos

	bulletObject(RigidBodyComponent* rb, Ogre::SceneNode* obj, int id) { _rb = rb, _node = obj; _id = id; }
};

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();	

	static PhysicsManager* Instance();

	void Update();
	void LateUpdate();

	void DetectCollision();

	//GETS	
	btDiscreteDynamicsWorld* getDynamicsWorld() { return _world; }
	std::deque<btCollisionShape*> getShapes() { return _shapes; }

	btRigidBody* CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float sizeX, float sizeY, float sizeZ, float rotX=1, float rotY=0, float rotZ=0);
	btRigidBody* CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float radius, float rotX = 1, float rotY = 0, float rotZ = 0);
	btRigidBody* CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float height, float radius, float rotX = 1, float rotY = 0, float rotZ = 0);
	btRigidBody* CreatePlaneCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float normalX, float normalY, float normalZ,float thickness, float rotX = 1, float rotY = 0, float rotZ = 0);
	

private:
	btDefaultCollisionConfiguration * _collisionConf;
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _broadphase;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _world;

	//Al parecer antes existian dos shapes distintas, la estatica y la dinámica, se concretó en una en función de la masa del rigidbody asociado
	std::deque<btCollisionShape*> _shapes;
	std::deque<btRigidBody*> _bodies;
	std::deque<bulletObject*> _bulletObjects;

	static PhysicsManager *instance;

	//Método que completa el objeto físico
	btRigidBody* CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor);

};