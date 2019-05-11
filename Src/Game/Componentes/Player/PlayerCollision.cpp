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
	// Si te golpea un enemigo baja tu salud
	if (id == EnemyID) {
		receiveDamage();
	}
	else if (id == HearthID) {
		gainHealth();
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


	if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_T))
		receiveDamage();
	else if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_Y))
		gainHealth();
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
	gameObject->getComponent<PlayerController>()->receiveDamage();
}

void PlayerCollision::gainHealth()
{
	gameObject->getComponent<PlayerController>()->gainHealth();
}
