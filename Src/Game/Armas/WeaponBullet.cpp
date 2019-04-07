#include "WeaponBullet.h"


WeaponBullet::WeaponBullet(GameObject* obj):Component(obj)
{
	//rb = new RigidBodyComponent(gameObject);
	//PhysicsManager::Instance()->CreateSphereCollider(rb, 0, gameObject->getNode(), 1, 
	//	4.85, 0.8+0.05, 4.6 + 5-0.5, 1, 1);
	rb = new RigidBodyComponent(gameObject);
	PhysicsManager::Instance()->CreateSphereCollider(rb, 0, gameObject->getNode(), 1,
		 gameObject->getNode()->getPosition().x-1, gameObject->getNode()->getPosition().y, gameObject->getNode()->getPosition().z, 1, 0.1);
	std::cout << gameObject->getNode()->getPosition();
}


WeaponBullet::~WeaponBullet()
{
}

void WeaponBullet::Update()
{
	rb->physicRB->applyCentralImpulse({ 0, 0, 1 });
}
