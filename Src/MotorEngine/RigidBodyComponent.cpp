#include "RigidBodyComponent.h"
#include "PhysicsManager.h"
#include <iostream>

void RigidBodyComponent::LoadFromFile(json obj)
{
	shape = obj["shape"];
	id = obj["id"];
	mass = obj["mass"];
	originalPosX = obj["positionX"];
	originalPosY = obj["positionY"];
	originalPosZ = obj["positionZ"];
	restitutionFactor = obj["restFactor"];
	
	offSetX = obj["offsetX"];
	offSetY = obj["offsetY"];
	offSetZ = obj["offsetZ"];

	isTrigger = obj["isTrigger"];

	switch (shape)
	{
		case 0: {
			sizeX = obj["sizeX"];
			sizeY = obj["sizeY"];
			sizeZ = obj["sizeZ"];			
			break;
		}
		case 1: {
			height = obj["height"];
			radius = obj["radius"];
			break;
		}
		case 2: {
			radius = obj["radius"];			
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

void RigidBodyComponent::Start()
{
	//Se crea y habilita por defecto el el rigidbody
	switch (shape)
	{
		case 0: {
			physicRB = PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z, restitutionFactor, sizeX, sizeY, sizeZ);
			break;
		}
		case 1: {
			physicRB = PhysicsManager::Instance()->CreateCapsuleCollider(this, id, gameObject->getNode(), mass, gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z, restitutionFactor, height, radius);
			break;
		}
		case 2: {
			physicRB = PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z, restitutionFactor, radius);
			break;
		}
	}
	cout << "Creado RB de objeto [ " << gameObject->getName() << " ] " << "en la posicion [ " << gameObject->getPosition() << " ]" << endl;
}

void RigidBodyComponent::OnDestroy()
{
	//Eliminado
	Component::OnDestroy();
	PhysicsManager::Instance()->removeRigidBody(gameObject->getNode());
}

void RigidBodyComponent::OnDisable()
{
	//Lo sacamos del mundo fisico
	PhysicsManager::Instance()->removeRigidBody(gameObject->getNode());
	//PhysicsManager::Instance()->removeFromWorld(physicRB);
}

void RigidBodyComponent::OnEnable()
{
	//Habilita el rb
	PhysicsManager::Instance()->addToWorld(physicRB);
}
