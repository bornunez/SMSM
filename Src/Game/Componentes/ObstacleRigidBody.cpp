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

	/*switch (shape)
	{
	case 0: {
		physicRB = PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, sizeX, sizeY, sizeZ, offSetX, offSetY, offSetZ);
		break;
	}
	case 1: {
		physicRB = PhysicsManager::Instance()->CreateCapsuleCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, height, radius, offSetX, offSetY, offSetZ);
		break;
	}
	case 2: {
		physicRB = PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, radius, offSetX, offSetY, offSetZ);
		break;
	}
	}*/

	isTrigger = false;		// Los obstaculos no reaccionan al contactos
}


void ObstacleRigidBody::collisionHandler(int id)
{

}

ObstacleRigidBody::~ObstacleRigidBody()
{
}