#pragma once
#include <btBulletDynamicsCommon.h>
#include "Game.h"
#include "myMotionState.h"
#include "RigidBodyComponent.h"

struct bulletObject {
	
	RigidBodyComponent* _rb;	// Permitir� llamar al collisionHandler del objeto al colisionar
	Ogre::SceneNode* _node;		// Servir� para detectar nuestro bullet object
	int _id;					// Servir� para saber el tipo de objeto que tenemos
	bool hit = false;			// Servir� para que la colision se detecte solo una vez, al cabo de x tiempo se restituir�
								// Es para evitar colisiones continuas

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

	btRigidBody* CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 size);
	btRigidBody* CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar radius);
	btRigidBody* CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar height, btScalar radius);
	btRigidBody* CreatePlaneCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 normalDir, btScalar thickness);
	

private:
	btDefaultCollisionConfiguration * _collisionConf;
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _broadphase;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _world;

	//Al parecer antes existian dos shapes distintas, la estatica y la din�mica, se concret� en una en funci�n de la masa del rigidbody asociado
	std::deque<btCollisionShape*> _shapes;
	std::deque<btRigidBody*> _bodies;
	std::deque<bulletObject*> _bulletObjects;

	static PhysicsManager *instance;

	//M�todo que completa el objeto f�sico
	btRigidBody* CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor);

};