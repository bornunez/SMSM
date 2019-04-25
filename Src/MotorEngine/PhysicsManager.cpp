#include "PhysicsManager.h"
#include <iostream>

PhysicsManager* PhysicsManager::instance = nullptr;

//Este manager inicializará el mundo fisico, y se encargará de comprobar las colisiones
PhysicsManager::PhysicsManager()
{
	std::cout << " -- Setting up the physics world -- " << std::endl;

	_collisionConf = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConf);
	_broadphase = new btDbvtBroadphase();
	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConf);

	_world->setGravity(btVector3(0, -40, 0));

#ifdef _DEBUG
	//debug_ = true;
#endif

	std::cout << " -- Physics world initialized -- " << std::endl;
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
	clearRigidBodies();

	_world->stepSimulation(1.f / 60.f, 1);

	for (int i = 0; i< _shapes.size(); i++) {
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState()) {
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			// Actualizamos el transform asociado a cada debugObject
			_debugObjects[i]._t = trans;

			void *userPointer = body->getUserPointer();
			if (userPointer) {
				btQuaternion orientation = trans.getRotation();
				Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);

				bulletObject* b = getBulletObject(sceneNode);

				if(b==nullptr)
					sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
				else
					sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX() - b->_offSet.x(), trans.getOrigin().getY() - b->_offSet.y(), trans.getOrigin().getZ() - b->_offSet.z()));
				
				sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
			}
		}
	}
	

	if (debug_) {
		myDebugDrawer::Instance()->clearLines();
		myDebugDrawer::Instance()->resetLineNumber();
	}
	CastAllRays();

	DetectCollision();

	

	if (debug_)
	{
		for (int i = 0; i < _debugObjects.size(); i++)
		{
			btVector3 v;
			v.setX(_debugObjects[i]._t.getOrigin().x());
			v.setY(_debugObjects[i]._t.getOrigin().y());
			v.setZ(_debugObjects[i]._t.getOrigin().z());

			if (_debugObjects[i]._id == 0)
				myDebugDrawer::Instance()->drawCube(v, _debugObjects[i]._scale);
		}
	}
}

void PhysicsManager::DetectCollision() {

	btDispatcher* dispatcher = _world->getCollisionWorld()->getDispatcher();
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

		// Solo mandamos el aviso si no son
		if (foundA && foundB) {
			if(_bulletObjects[Aidx]->_rb->getIsTrigger())
				_bulletObjects[Aidx]->_rb->collisionHandler(_bulletObjects[Bidx]->_id);
			if(_bulletObjects[Bidx]->_rb->getIsTrigger())
				_bulletObjects[Bidx]->_rb->collisionHandler(_bulletObjects[Aidx]->_id);
		}
	}
}



btRigidBody * PhysicsManager::CreateBoxCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float sizeX, float sizeY, float sizeZ, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btBoxShape(btVector3(sizeX, sizeY, sizeZ));
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id, btVector3(offsetX, offsetY, offsetZ));
	_bulletObjects.push_back(b);

	btVector3 v(sizeX, sizeY, sizeZ);
	CreateDebugObject(node, 0, 0, v);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreateSphereCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float radius, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btSphereShape(radius);
	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id, btVector3(offsetX, offsetY, offsetZ));
	_bulletObjects.push_back(b);

	btVector3 v(radius, radius, radius);
	CreateDebugObject(node, 0, 0, v);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreateCapsuleCollider(RigidBodyComponent* rb, int id, SceneNode * node, float mass, float posX, float posY, float posZ, float restitutionFactor, float height, float radius, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ)
{
	btCollisionShape *newRigidShape;
	newRigidShape = new btCapsuleShape(height, radius);

	_shapes.push_back(newRigidShape);

	bulletObject* b = new bulletObject(rb, node, id, btVector3(offsetX, offsetY, offsetZ));
	_bulletObjects.push_back(b);

	btVector3 v(radius, height, radius);
	CreateDebugObject(node, 0, 0, v);

	return CreatePhysicObject(newRigidShape, node, mass, btVector3(posX, posY, posZ), btQuaternion(rotX, rotY, rotZ, 0), restitutionFactor);
}

btRigidBody * PhysicsManager::CreateMeshCollider(RigidBodyComponent * rb, int id, SceneNode * node, string mesh, float mass, float posX, float posY, float posZ, float restitutionFactor, float scale, float rotX, float rotY, float rotZ)
{

	//btConvexHullShape convexHullShape( m_pPositions->GetVertexData(), m_pPositions->GetNumVertices(), m_pPositions->GetStride() * sizeof(float));
	////create a hull approximation
	//convexHullShape.setMargin(0);  // this is to compensate for a bug in bullet
	//btShapeHull* hull = new btShapeHull(&convexHullShape);
	//hull->buildHull(0);    // note: parameter is ignored by buildHull
	//btConvexHullShape* pConvexHullShape = new btConvexHullShape(
	//	(const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
	//m_pDynamicShape = pConvexHullShape;
	//delete hull;
	return nullptr;
}

void PhysicsManager::removeRigidBody(SceneNode * node)
{
	rigidBodiesToRemove_.push_back(node);
}

void PhysicsManager::clearRigidBodies()
{
	for (int h = 0; h < rigidBodiesToRemove_.size(); h++) {
		int j = 0;

		bool found = false;
		int idx = 0;

		while (j < _bulletObjects.size() && !found) {

			if (_bulletObjects[j]->_node == rigidBodiesToRemove_[h]) {
				idx = j;
				found = true;
			}
			else
				j++;
		}

		auto itShapes = _shapes.begin();
		auto itBodies = _bodies.begin();
		auto itBulletObj = _bulletObjects.begin();
		auto itDebugObj = _debugObjects.begin();

		for (int i = 0; i < idx; i++) {
			++itShapes;
			++itBodies;
			++itBulletObj;
			++itDebugObj;
		}

		_world->removeRigidBody(_bodies[idx]);

		_shapes.erase(itShapes);

		_bodies.erase(itBodies);

		_debugObjects.erase(itDebugObj);

		_bulletObjects[idx]->clearBulletObject();
		_bulletObjects.erase(itBulletObj);
	}

	rigidBodiesToRemove_.clear();
}

void PhysicsManager::CreateRaycast(btVector3 from, btVector3 to, bool hit, string name)
{
	rayCast r;
	r.from_ = from;
	r.to_ = to;
	r.hit_ = hit;
	r.name_ = name;

	rayCasts_.push_back(r);
}

void PhysicsManager::updateRaycast(btVector3 from, btVector3 to, std::string name)
{
	bool found = false;
	int i = 0;

	while (i < rayCasts_.size() && !found)
	{
		if (name == rayCasts_[i].name_)
		{
			rayCasts_[i].from_ = from;
			rayCasts_[i].to_ = to;
			rayCasts_[i].hit_ = false;
			found = true;
		}
		else i++;
	}
}

void PhysicsManager::getRaycast(btVector3 & from, btVector3 & to, bool& hit, std::string name)
{
	bool found = false;
	int i = 0;

	while (i < rayCasts_.size() && !found)
	{
		if (name == rayCasts_[i].name_)
		{
			from = rayCasts_[i].from_;
			to = rayCasts_[i].to_;
			hit = rayCasts_[i].hit_;
			found = true;
		}
		else i++;
	}
}


std::deque<Ogre::SceneNode*> PhysicsManager::CastAllRays()
{
	std::deque<Ogre::SceneNode*> collisionNodes;

	if (_world) {

		_world->updateAabbs();
		_world->computeOverlappingPairs();

		for (int i = 0; i < rayCasts_.size(); i++) {

			if (rayCasts_[i].hit_)
				allHitRaycast(rayCasts_[i].from_, rayCasts_[i].to_);
			else
				collisionNodes.push_back(firstRaycastHit(rayCasts_[i].from_, rayCasts_[i].to_));
		}
	}

	return collisionNodes;
}

void PhysicsManager::allHitRaycast(btVector3 from, btVector3 to)
{
	_world->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 0, 1));
	btCollisionWorld::AllHitsRayResultCallback results(from, to);
	results.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
	results.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

	_world->rayTest(from, to, results);

	for (int i = 0; i < results.m_hitFractions.size(); i++)
	{
		if (debug_)
		{
			btVector3 p = from.lerp(to, results.m_hitFractions[i]);
			myDebugDrawer::Instance()->drawCube(p, btVector3(1, 1, 1));
		}
	}
}

Ogre::SceneNode * PhysicsManager::firstRaycastHit(btVector3 from, btVector3 to)
{
	btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	_world->rayTest(from, to, closestResults);
	Ogre::SceneNode* collisionNode = nullptr;

	if (closestResults.hasHit()) {

		const btCollisionObject* collidedObj = closestResults.m_collisionObject;

		const btRigidBody* body = btRigidBody::upcast(collidedObj);

		collisionNode = static_cast<Ogre::SceneNode*>(body->getUserPointer());

	}

	return collisionNode;
}

void PhysicsManager::CreateDebugObject(Ogre::SceneNode * node, int id, float radius, btVector3 scale)
{
	debugObjects debugObj;
	debugObj._node = node; debugObj._radius = radius; debugObj._scale = scale; debugObj._id = id;

	_debugObjects.push_back(debugObj);
}

void PhysicsManager::debugMode()
{
	dbg_drawer = myDebugDrawer::Instance();	// DebugDrawer derives btIDebugDraw, see below for my definition
	_world->setDebugDrawer(dbg_drawer);
}

void PhysicsManager::toggleDebug()
{
	debug_ = !debug_;
}


void PhysicsManager::addToWorld(btRigidBody *body)
{
	_world->addRigidBody(body);
	_bodies.push_back(body);
}

void PhysicsManager::removeFromWorld(btRigidBody * body)
{
	_world->removeRigidBody(body);
	auto it = _bodies.begin();
	bool found = false;
	int i = 0;

	while (!found && it!=_bodies.end()) {

		btRigidBody* b = _bodies.at(i);

		if (b == body) {
			found = true;
		}
		else {
			++it;
			i++;
		}
	}

	if (found) 
		_bodies.erase(it);	
	else
		std::cout << "No se encuentra el rigidbody en el array" << std::endl;

}

bulletObject * PhysicsManager::getBulletObject(Ogre::SceneNode * node)
{
	bool found = false;
	int j = 0;

	while (j < _bulletObjects.size() && !found) {

		if (_bulletObjects[j]->_node == node) {
			found = true;
		}
		else
			j++;
	}
	if (found)
		return _bulletObjects[j];
	else
		return nullptr;
}

btRigidBody * PhysicsManager::CreatePhysicObject(btCollisionShape* collisionShape, SceneNode * node, btScalar mass, btVector3 originalPosition, btQuaternion originalRotation, btScalar restitutionFactor)
{
	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(originalPosition);
	collisionShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *state = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, state, collisionShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(restitutionFactor);


	_world->addRigidBody(body);
	_bodies.push_back(body);
	body->setUserPointer(node);

	return body;
}