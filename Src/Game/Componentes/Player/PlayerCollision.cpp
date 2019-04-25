#include "PlayerCollision.h"

PlayerCollision::PlayerCollision(GameObject * obj) : RigidBodyComponent(obj)
{
}

PlayerCollision::~PlayerCollision()
{
}

//void PlayerCollision::LoadFromFile(json obj)
//{
//	RigidBodyComponent::LoadFromFile(obj);	
//}

void PlayerCollision::collisionHandler(int id)
{
	
}

void PlayerCollision::Update()
{
	btTransform trans;
	physicRB->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion(0 ,trans.getRotation().y(), 0, trans.getRotation().w()));

	gameObject->getNode()->setOrientation(trans.getRotation().w(),trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z());
}

void PlayerCollision::Start()
{
	RigidBodyComponent::Start();	//Se crea aqui y se asigna el physicRbs	
	physicRB->setRollingFriction(0);
	physicRB->setFriction(0);
}
