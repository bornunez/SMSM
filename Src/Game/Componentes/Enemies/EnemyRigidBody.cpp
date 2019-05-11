#include "EnemyRigidBody.h"
#include "../../../Src/MotorEngine/PhysicsManager.h"
#include <iostream>

void EnemyRigidBody::Start() {

	enemy = gameObject->getComponent<Enemy>();
	RigidBodyComponent::Start();
}

EnemyRigidBody::~EnemyRigidBody()
{
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
	if (gravity != -1)	// Si no se ajusta de forma continuada, pasa a afectarle la gravedad del mundo normal
		physicRB->setGravity(btVector3(0, gravity, 0));

	btTransform trans;
	physicRB->getMotionState()->getWorldTransform(trans);

	trans.setOrigin(btVector3(trans.getOrigin().x() - 1, trans.getOrigin().y(), trans.getOrigin().z()));

	//gameObject->getNode()->setPosition(trans.getOrigin().x() + offSetX, trans.getOrigin().y() + offSetY, trans.getOrigin().z() + offSetZ);
}

