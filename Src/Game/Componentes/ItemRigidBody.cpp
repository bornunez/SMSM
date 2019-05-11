#include "ItemRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"

void  ItemRigidBody::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	if (!obj.contains("filePos")) {
		originalPosX = gameObject->getNode()->getPosition().x + obj["offsetX"];
		originalPosY = gameObject->getNode()->getPosition().y + obj["offsetY"];
		originalPosZ = gameObject->getNode()->getPosition().z + obj["offsetZ"];
	}
}


void  ItemRigidBody::collisionHandler(int id)
{
	// Si el jugador lo coge se destruye
	if (id == 0) {
		gameObject->Destroy();
	}
}

ItemRigidBody::~ItemRigidBody()
{
}
