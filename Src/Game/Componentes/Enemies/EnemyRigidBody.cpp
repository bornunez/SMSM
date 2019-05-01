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
	if (!hit) {
		
		hit = true;
		// Si la ID es la ID de la bala.
		if (id == 1) {
			std::cout << "--> Ha colisionado conmigo un objeto con el identificador: " + to_string(id) << " <--" << std::endl;
			cout << "llamando al ON HIT" << endl;
			enemy->OnHit();
		}
	}
	else {
		//Para evitar colisiones constantes, si queremos colisiones constantes, basta con reducir el recoil a 0
		//std::cout << "Todav�a estoy en recoil" << std::endl;
	}
}

void EnemyRigidBody::Update()
{
	btTransform trans;
	physicRB->getMotionState()->getWorldTransform(trans);

	trans.setOrigin(btVector3(trans.getOrigin().x() - 1, trans.getOrigin().y(), trans.getOrigin().z()));

	gameObject->getNode()->setPosition(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
}

EnemyRigidBody::~EnemyRigidBody()
{
}
