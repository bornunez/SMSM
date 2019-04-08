#include "PlayerCollision.h"

PlayerCollision::PlayerCollision(GameObject * obj) : RigidBodyComponent(obj)
{
}

PlayerCollision::~PlayerCollision()
{
}

void PlayerCollision::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);
	physicRB = PhysicsManager::Instance()->CreateCapsuleCollider(this, id, gameObject->getNode(), mass, gameObject->getNode()->getPosition().x, gameObject->getNode()->getPosition().y, gameObject->getNode()->getPosition().z, restitutionFactor, height, radius, offSetX, offSetY, offSetZ);
	physicRB->setRollingFriction(0);
	physicRB->setFriction(0);
}

void PlayerCollision::collisionHandler(int id)
{
	
}

void PlayerCollision::Update()
{
	btTransform trans;
	physicRB->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion(0 ,trans.getRotation().y(), 0, trans.getRotation().w()));

	gameObject->getNode()->setOrientation(trans.getRotation().w(),trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
}
