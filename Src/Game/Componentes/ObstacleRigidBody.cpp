#include "ObstacleRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"

void ObstacleRigidBody::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	if (!obj.contains("filePos")) {
		originalPosX = gameObject->getNode()->getPosition().x + obj["offsetX"];
		originalPosY = gameObject->getNode()->getPosition().y + obj["offsetY"];
		originalPosZ = gameObject->getNode()->getPosition().z + obj["offsetZ"];
	}
	isTrigger = false;		// Los obstaculos no reaccionan al contactos
}


void ObstacleRigidBody::collisionHandler(int id)
{

}

ObstacleRigidBody::~ObstacleRigidBody()
{
}