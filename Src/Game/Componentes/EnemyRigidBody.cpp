#include "EnemyRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"
#include <iostream>

void EnemyRigidBody::LoadFromFile(json obj)
{
	int shape = obj["shape"];
	int id = obj["id"];
	float mass = obj["mass"];
	float originalPosX = obj["positionX"];
	float originalPosY = obj["positionY"];
	float originalPosZ = obj["positionZ"];
	float restitutionFactor = obj["restFactor"];

	//Parametros
	recoilTime = obj["recoilTime"];

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
			float normalX = obj["normalX"];
			float normalY = obj["normalY"];
			float normalZ = obj["normalZ"];
			float thickness = obj["thickness"];
			PhysicsManager::Instance()->CreatePlaneCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, normalX, normalY, normalZ, thickness);
			break;
		}
		case 3: {
			float radius = obj["radius"];
			PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, originalPosX, originalPosY, originalPosZ, restitutionFactor, radius);
			break;
		}
	}
}

void EnemyRigidBody::collisionHandler(int id)
{
	string id_;
	if (id == 0)
		id_ = "0";
	else if (id == 1)
		id_ = "1";
	else if (id == 2)
		id_ = "2";
	else if (id == 5)
		id_ = "5";
	else
		id_ = "OTHER NUMBER";

	if (!hit) {
		std::cout << "--> Ha colisionado conmigo un objeto con el identificador: " + id_ << " <--" << std::endl;
		hit = true;
	}
	else {
		//Para evitar colisiones constantes, si queremos colisiones constantes, basta con reducir el recoil a 0
		//std::cout << "Todav�a estoy en recoil" << std::endl;
	}
}

EnemyRigidBody::~EnemyRigidBody()
{
}