#pragma once
#include <btBulletDynamicsCommon.h>
#include "../../../bullet-2.82-r2704/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "../../../bullet-2.82-r2704/src/BulletCollision/Gimpact/btGImpactShape.h"
#include "myDebugDrawer.h"

#include "Game.h"
#include "RigidBodyComponent.h"
#include <map>
#include <vector>


class bulletObject {

public:
	RigidBodyComponent * _rb;	// Permitir� llamar al collisionHandler del objeto al colisionar
	Ogre::SceneNode* _node;		// Servir� para detectar nuestro bullet object
	int _id;					// Servir� para saber el tipo de objeto que tenemos

	bulletObject(RigidBodyComponent* rb, Ogre::SceneNode* obj, int id) { _rb = rb, _node = obj; _id = id; }
	void clearBulletObject() { _rb = nullptr; _node = nullptr; }
};

struct debugObjects {
	Ogre::SceneNode* _node;
	btVector3 _scale;
	double _radius;
	int _id;
};

struct rayCast {

	btVector3 from_;
	btVector3 to_;
	bool hit_;
	std::string name_;
};

class PhysicsManager
{
public:
	PhysicsManager();
	virtual ~PhysicsManager();

	static PhysicsManager* Instance();

	void Update();

	void DetectCollision();

	//GETS	
	btDiscreteDynamicsWorld* getDynamicsWorld() { return _world; }
	std::deque<btCollisionShape*> getShapes() { return _shapes; }

	//RigidBodies

	//A�adir
	btRigidBody* CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float sizeX, float sizeY, float sizeZ, float rotX = 1, float rotY = 0, float rotZ = 0);
	btRigidBody* CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float radius, float rotX = 1, float rotY = 0, float rotZ = 0);
	btRigidBody* CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float height, float radius, float rotX = 1, float rotY = 0, float rotZ = 0);
	
	//Eliminar
	void removeRigidBody(SceneNode * node); //Para eliminar el rigidbody asociado a un nodo de la escena
	void clearRigidBodies();				//Elimina los rigidbodies asociados a los nodos al final del update

											//Raycasts
	void CreateRaycast(btVector3 from, btVector3 to, bool hit, std::string name);
	void updateRaycast(btVector3 from, btVector3 to, std::string name);
	void getRaycast(btVector3& from, btVector3& to, bool& hit, std::string name);


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
	std::deque<Ogre::SceneNode*> rigidBodiesToRemove_;


	//RAYCASTS

	std::deque<Ogre::SceneNode*> CastAllRays();			//M�todo para coger el primer objeto que colisiona con el rayo	
	void allHitRaycast(btVector3 from, btVector3 to);	//Metodo para mostrar visualmente

	Ogre::SceneNode* firstRaycastHit(btVector3 from, btVector3 to);
	std::deque<rayCast> rayCasts_;


	//DEBUG
	bool debug_ = false;
	myDebugDrawer* dbg_drawer;
	void CreateDebugObject(Ogre::SceneNode * node, int id, float radius, btVector3 scale);

	void debugMode();
	void toggleDebug();
	std::deque<debugObjects> _debugObjects;
};