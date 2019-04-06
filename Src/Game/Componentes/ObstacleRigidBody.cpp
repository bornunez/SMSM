#include "ObstacleRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"

void ObstacleRigidBody::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	isTrigger = false;		// Los obstaculos no reaccionan al contactos
}

void ObstacleRigidBody::collisionHandler(int id)
{

}

ObstacleRigidBody::~ObstacleRigidBody()
{
}