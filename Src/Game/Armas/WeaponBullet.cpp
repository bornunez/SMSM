#include "WeaponBullet.h"


WeaponBullet::WeaponBullet(GameObject* obj):RigidBodyComponent(obj)
{

}


WeaponBullet::~WeaponBullet()
{
}

void WeaponBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);
	physicRB = PhysicsManager::Instance()->CreateSphereCollider(this, id, gameObject->getNode(), mass, 
		gameObject->getNode()->getPosition().x, gameObject->getNode()->getPosition().y, gameObject->getNode()->getPosition().z, 
		restitutionFactor, radius, offSetX, offSetY, offSetZ);
	
	//Params from file
	physicRB->setGravity(btVector3(0, obj["gravity"], 0));
	physicRB->setDamping(obj["linDamp"], obj["angDamp"]);
	speed = obj["speed"];
	physicRB->applyTorqueImpulse({ 0, 0.1, 0 });
}

void WeaponBullet::collisionHandler(int id)
{
	if (id != 0)
	{
		if (!hit) {
			std::cout << "--> SOY UNA BALA Y HE COLISIONADO <--" << std::endl;
			hit = true;
			physicRB->clearForces();
		}
	}
}

void WeaponBullet::Update()
{
	if(!hit)
		physicRB->applyCentralImpulse({ 0, 0, -speed });
	else //Se posria hacer aqui un contador para que desapareciese la bala
	{
		
	}
}
