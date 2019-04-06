#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include <iostream>

void RigidBodyComponent::LoadFromFile(json obj)
{
	int shape = obj["shape"];
	int id = obj["id"];
	float mass = obj["mass"];
	float originalPosX = obj["positionX"];
	float originalPosY = obj["positionY"];
	float originalPosZ = obj["positionZ"];
	float restitutionFactor = obj["restFactor"];
	
	//float rotX = obj["rotationX"];
	//float rotY = obj["rotationY"];
	//float rotZ = obj["rotationZ"];

	float offSetX = obj["offsetX"];
	float offSetY = obj["offsetY"];
	float offSetZ = obj["offsetZ"];

	isTrigger = obj["isTrigger"];

	switch (shape)
	{
		case 0: {
			float sizeX = obj["sizeX"];
			float sizeY = obj["sizeY"];
			float sizeZ = obj["sizeZ"];
			physicRB = PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, sizeX, sizeY, sizeZ, offSetX, offSetY, offSetZ);
			break;
		}
		case 1: {
			float height = obj["height"];
			float radius = obj["radius"];
			physicRB = PhysicsManager::Instance()->CreateCapsuleCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, height, radius, offSetX, offSetY, offSetZ);
			break;
		}
		case 2: {
			float radius = obj["radius"];
			physicRB = PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, radius, offSetX, offSetY, offSetZ);
			break;
		}
	}

	if (shape > 2) {
		std::cout << " ** Indice inexistente de forma (shape) en la constructora ** " << std::endl;
	}
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Update()
{
	resetHitState();
}

void RigidBodyComponent::resetHitState()
{
	if (isTrigger) {
		if (hit && timeSinceLastHit > recoilTime)
		{
			hit = false;
			timeSinceLastHit = 0;
		}
		else
		{
			timeSinceLastHit += TimeManager::getInstance()->getDeltaTime();
		}
	}
}
