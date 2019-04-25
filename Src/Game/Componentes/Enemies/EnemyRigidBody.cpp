#include "EnemyRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"
#include <iostream>

void EnemyRigidBody::Start() {

	std::list<Component*> comps = gameObject->getComponents();
	bool found = false;
	auto it = comps.begin();
	
	while (!found && it != comps.end())
	{
		Enemy* c = dynamic_cast<Enemy*>(*it);
		if (c != nullptr) {
			found = true;
			enemy = c;
		}
		it++;
	}

	RigidBodyComponent::Start();
}

void EnemyRigidBody::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);
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
	else if (id == 7) {
		id_ = "7";
	}
	else
		id_ = "OTHER NUMBER";

	if (!hit) {
		std::cout << "--> Ha colisionado conmigo un objeto con el identificador: " + id_ << " <--" << std::endl;
		hit = true;
		if (id_ == "7") {
			enemy->OnHit();
		}
	}
	else {
		//Para evitar colisiones constantes, si queremos colisiones constantes, basta con reducir el recoil a 0
		//std::cout << "Todav�a estoy en recoil" << std::endl;
	}
}

EnemyRigidBody::~EnemyRigidBody()
{
}
