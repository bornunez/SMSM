#include "PlayerCollision.h"
#include "PlayerController.h"

PlayerCollision::PlayerCollision(GameObject * obj) : RigidBodyComponent(obj)
{
}

PlayerCollision::~PlayerCollision()
{
}

void PlayerCollision::collisionHandler(int id)
{
	if (id == 5) {
#ifdef C_DEBUG
		cout << "OUCH OUCH ME DISPARARON" << endl;
#endif // C_DEBUG
	}
}

void PlayerCollision::Update()
{
	//cout << " RB de objeto [ " << gameObject->getName() << " ] " << "en la posicion [ " << gameObject->getPosition() << " ]" << endl;
	btTransform trans;
	physicRB->getMotionState()->getWorldTransform(trans);


	trans.setOrigin(btVector3(trans.getOrigin().x()-1, 0.6, trans.getOrigin().z()));
	trans.setRotation(btQuaternion(0,trans.getRotation().y(), 0, trans.getRotation().w()));

	gameObject->getNode()->setOrientation(trans.getRotation().w(),trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
	//gameObject->getNode()->setPosition(trans.getOrigin().x() + offSetX, trans.getOrigin().y() + offSetY, trans.getOrigin().z() + offSetZ);

}

void PlayerCollision::Start()
{
	RigidBodyComponent::Start();	//Se crea aqui y se asigna el physicRbs	
	physicRB->setRollingFriction(0);
	physicRB->setFriction(0);
	physicRB->setAngularFactor(btVector3(0, 1, 0));
}

void PlayerCollision::receiveDamage()
{
	if (canGetDamage) {
		canGetDamage = false;
		
		gameObject->getComponent<PlayerController>()->

	}
}
