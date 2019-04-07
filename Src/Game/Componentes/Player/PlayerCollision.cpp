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
	physicRB = PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, gameObject->getNode()->getPosition().x, gameObject->getNode()->getPosition().y, gameObject->getNode()->getPosition().z, restitutionFactor, sizeX, sizeY, sizeZ, offSetX, offSetY, offSetZ);
}

void PlayerCollision::collisionHandler(int id)
{
}

void PlayerCollision::Update()
{
}
