#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include <iostream>

void RigidBodyComponent::LoadFromFile(json obj)
{
	int shape = obj["shape"];
	int id = obj["id"];
	float mass = obj["mass"];				// Masa a cero para objetos estaticos
	std::cout << gameObject->getPosition().x << std::endl;
	std::cout << gameObject->getPosition().y << std::endl;
	std::cout << gameObject->getPosition().x << std::endl;
	float originalPosX = gameObject->getPosition().x;//obj["positionX"];
	float originalPosY = gameObject->getPosition().y; //obj["positionY"];
	float originalPosZ = gameObject->getPosition().z; //obj["positionZ"];
	float restitutionFactor = obj["restFactor"];
	isTrigger = obj["isTrigger"];

	switch (shape)
	{
	case 0: {
		float sizeX = obj["sizeX"];
		float sizeY = obj["sizeY"];
		float sizeZ = obj["sizeZ"];
		PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, sizeX, sizeY, sizeZ);
		break;
	}
	case 1: {
		float height = obj["height"];
		float radius = obj["radius"];
		PhysicsManager::Instance()->CreateCapsuleCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, height, radius);
		break;
	}
	case 2: {
		float radius = obj["radius"];
		PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, radius);
		break;
	}
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
