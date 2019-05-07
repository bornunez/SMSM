#include "EnemyRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"
#include <iostream>

void EnemyRigidBody::Start() {

	enemy = gameObject->getComponent<Enemy>();
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
		// Si la ID es la ID de la bala.
		if (id == 1) {
#ifdef C_DEBUG
			std::cout << "--> Ha colisionado conmigo un objeto con el identificador: " + to_string(id) << " <--" << std::endl;
			cout << "llamando al ON HIT" << endl;
#endif
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
