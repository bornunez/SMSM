#include "PhysicsManager.h"
#include <iostream>

PhysicsManager* PhysicsManager::instance = nullptr;

//Este manager inicializará el mundo fisico, y se encargará de comprobar las colisiones
PhysicsManager::PhysicsManager()
{
	_collisionConf = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConf);
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConf);

	_world->setGravity(btVector3(0, -30, 0));
}


PhysicsManager::~PhysicsManager()
{
	std::deque<btRigidBody*>::iterator itBody = _bodies.begin();

	while (itBody != _bodies.end()) {
		delete *itBody;
		++itBody;
	}

	std::deque<btCollisionShape*>::iterator itShape = _shapes.begin();

	while (itShape != _shapes.end()) {
		delete *itShape;
		++itShape;
	}

	_bodies.clear();
	_shapes.clear();

	delete _world;
	delete _solver;
	delete _broadphase;
	delete _dispatcher;
	delete _collisionConf;
}

PhysicsManager* PhysicsManager::Instance()
{
	if (instance == nullptr) {
		instance = new PhysicsManager();
	}
	
	return instance;
}

void PhysicsManager::Update()
{
	_world->stepSimulation(1.f / 60.f, 1);

	for (int i = 0; i< _shapes.size(); i++) {
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState()) {
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			void *userPointer = body->getUserPointer();
			if (userPointer) {
				btQuaternion orientation = trans.getRotation();
				Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
				sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
				sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
			}
		}
	}

	DetectCollision();
}

void PhysicsManager::LateUpdate()
{
	//_world->stepSimulation(1.f / 60.f, 10);
}

void PhysicsManager::DetectCollision() {
	
	//btCollisionWorld *bulletWorld = _world->getCollisionWorld();
	btDispatcher* dispatcher = _world->getCollisionWorld()->getDispatcher();
	//int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();
	int numManifolds = dispatcher->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold * contactManifold = dispatcher->getManifoldByIndexInternal(i);

		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		
		int j = 0;

		bool foundA = false;
		int Aidx = 0;
		bool foundB = false;
		int Bidx = 0;

		while (j < _bulletObjects.size() && (!foundA || !foundB)) {
			
			if (!foundA && static_cast<Ogre::SceneNode *>(obA->getUserPointer()) == _bulletObjects[j]->_node) {
				Aidx = j;
				foundA = true;
			}
			else if (!foundB && static_cast<Ogre::SceneNode *>(obB->getUserPointer()) == _bulletObjects[j]->_node) {
				Bidx = j;
				foundB = true;
			}
			j++;
		}

		for (int k = 0; k < contactManifold->getNumContacts(); k++) {

			//contactManifold->removeContactPoint(k);
			contactManifold->refreshContactPoints(obA->getWorldTransform(), obB->getWorldTransform());

		}

		if (foundA && foundB) {			
			_bulletObjects[Aidx]->_rb->collisionHandler(_bulletObjects[Bidx]->_id);
			_bulletObjects[Bidx]->_rb->collisionHandler(_bulletObjects[Aidx]->_id);
		}
	}


}



btRigidBody * PhysicsManager::CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float sizeX, float sizeY, float sizeZ, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btBoxShape(btVector3(sizeX,sizeY,sizeZ));
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX,posY,posZ), btQuaternion(rotX,rotY,rotZ,0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float radius, float rotX , float rotY, float rotZ )
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btSphereShape(radius);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float height, float radius, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btCapsuleShape(height,radius);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreatePlaneCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float normalX, float normalY, float normalZ, float thickness, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btStaticPlaneShape(btVector3(normalX,normalY,normalZ), thickness);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}


btRigidBody * PhysicsManager::CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor)
{
	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setRotation(originalRotation);
	
	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(originalPosition);
	collisionShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *state = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, collisionShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(restitutionFactor);
	

	_world->addRigidBody(body);

	body->setUserPointer(node);

	return body;
}
