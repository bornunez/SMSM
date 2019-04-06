#include "EnemyRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"
#include <iostream>

void EnemyRigidBody::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Parametros
	recoilTime = obj["recoilTime"];
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
		//std::cout << "Todavía estoy en recoil" << std::endl;
	}
}

EnemyRigidBody::~EnemyRigidBody()
{
}
