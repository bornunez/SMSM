#include "PhysicsManager.h"

#include <iostream>

//Este manager inicializará el mundo fisico, y se encargará de comprobar las colisiones
PhysicsManager::PhysicsManager()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -30, 0));
}


PhysicsManager::~PhysicsManager()
{
	for (int j = 0; j<collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
	collisionShapes.clear();
}

void PhysicsManager::Update()
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	for (int i = 0; i< collisionShapes.size(); i++) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
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
}

void PhysicsManager::addRigidBody(btRigidBody* rb)
{
	dynamicsWorld->addRigidBody(rb);
}

void PhysicsManager::addCollisionShape(btCollisionShape * sh)
{
	collisionShapes.push_back(sh);
}

void PhysicsManager::Move(btRigidBody * rb, SceneNode* node, btVector3 dir, float speed)
{
	btTransform m;
	rb->getMotionState()->getWorldTransform(m);

	m.setOrigin(m.getOrigin() + dir * speed);
	rb->getMotionState()->setWorldTransform(m);
	rb->setWorldTransform(m);
	node->setPosition(Ogre::Vector3(m.getOrigin().getX(), m.getOrigin().getY(), m.getOrigin().getZ()));
}

void PhysicsManager::Rotate(btRigidBody * rb, SceneNode* node, btQuaternion rot, float speed)
{
	btTransform m;
	btQuaternion orientation = m.getRotation();
	rb->getMotionState()->getWorldTransform(m);

	m.setRotation(m.getRotation() + rot * speed);
	rb->getMotionState()->setWorldTransform(m);
	rb->setWorldTransform(m);
	node->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
}

void PhysicsManager::AddDiscreteImpulse(btVector3 dir, btScalar strenght, int group)
{
	if(group < rigidBodiesGroups.size())
		for (int i = 0; i < rigidBodiesGroups[group].size(); i++) {
			rigidBodiesGroups[group][i]->applyCentralImpulse(dir * strenght);
		}
}

//Añade un rigidbody a un grupo concreto para trabajar con ese conjunto más facilmente
//Indicar un -1 en el grupo, implica añadir un nuevo grupo
//Devuelve el grupo al que pertenece, grupos ordenados a partir de 0
int PhysicsManager::AddRigidBodyToGroup(btRigidBody * rb, int group)
{
	if (group == -1 || group >= rigidBodiesGroups.size()) {
		rigidBodiesGroups.push_back(std::vector<btRigidBody*>());
		rigidBodiesGroups[rigidBodiesGroups.size() - 1].push_back(rb);
		return rigidBodiesGroups.size() - 1;
	}
	else {
		rigidBodiesGroups[group].push_back(rb);
		return group;
	}
}



btRigidBody * PhysicsManager::CreateBoxCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 size)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btBoxShape(size);
	addCollisionShape(newRigidShape);
	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreateSphereCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar radius)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btSphereShape(radius);
	addCollisionShape(newRigidShape);
	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreateCapsuleCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btScalar height, btScalar radius)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btCapsuleShape(height,radius);
	addCollisionShape(newRigidShape);
	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

btRigidBody * PhysicsManager::CreatePlaneCollider(SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor, btVector3 normalDir, btScalar size)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btStaticPlaneShape(normalDir, size);
	addCollisionShape(newRigidShape);
	return CreatePhysicObject(newRigidShape, node, mass, originalPosition, originalRotation, restitutionFactor);
}

//set the mass of the object. a mass of "0" means that it is an immovable object
btRigidBody * PhysicsManager::CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor)
{
	//set the initial position and transform. For this demo, we set the tranform to be none
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(originalRotation);
	
	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(originalPosition);
	collisionShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(restitutionFactor);
	body->setUserPointer(node);

	dynamicsWorld->addRigidBody(body);
	return body;
}
