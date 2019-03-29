#include "PhysicsManager.h"
#include <iostream>

//Este manager inicializará el mundo fisico, y se encargará de comprobar las colisiones
PhysicsManager::PhysicsManager()
{
	_collisionConf = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConf);
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConf);

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

void PhysicsManager::Update()
{
	_world->stepSimulation(1.f / 60.f, 10);

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
	
	btCollisionWorld *bulletWorld = _world->getCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold * contactManifold = bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		
		int j = 0;
		bool foundA = false;
		int Aidx;
		bool foundB = false;
		int Bidx;

		while (j < _bulletObjects.size() && (!foundA || !foundB)) {
			
			if (static_cast<Ogre::SceneNode *>(obA->getUserPointer()) == _bulletObjects[i]->_node) {
				Aidx = j;
				foundA = true;
			}
			else if (static_cast<Ogre::SceneNode *>(obB->getUserPointer()) == _bulletObjects[i]->_node) {
				Bidx = j;
				foundB = true;
			}
			j++;
		}

		if (foundA && foundB) {
			_bulletObjects[Aidx]->_rb->collisionHandler(_bulletObjects[Bidx]->_id);
			_bulletObjects[Bidx]->_rb->collisionHandler(_bulletObjects[Aidx]->_id);
		}
	}
}



btRigidBody * PhysicsManager::CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 size)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btBoxShape(size);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar radius)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btSphereShape(radius);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar height, btScalar radius)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btCapsuleShape(height,radius);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreatePlaneCollider(RigidBodyComponent* rb, int id, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 normalDir, btScalar thickness)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btStaticPlaneShape(normalDir, thickness);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id);
	_bulletObjects.push_back(b);

	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}


btRigidBody * PhysicsManager::CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor)
{
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(originalRotation);
	
	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(originalPosition);
	collisionShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	myMotionState *state = new myMotionState(startTransform, node);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, collisionShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(restitutionFactor);
	body->setUserPointer(node);

	_world->addRigidBody(body);
	return body;
}
