#include "WeaponBullet.h"


WeaponBullet::WeaponBullet(GameObject* obj):RigidBodyComponent(obj)
{

}


WeaponBullet::~WeaponBullet()
{
}

void WeaponBullet::Start()
{
	RigidBodyComponent::Start();
	physicRB->setGravity(btVector3(0, grav, 0));
	physicRB->setDamping(linDamp, angDamp);
	physicRB->applyTorqueImpulse(btVector3(0, 0.1, 0));
	direccion = scene->getGameObject("Player")->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;

}

void WeaponBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Params from file
	grav = obj["gravity"];
	linDamp = obj["linDamp"];
	angDamp = obj["angDamp"];
	speed = obj["speed"];
	recoilTime = obj["recoilTime"];
}

void WeaponBullet::collisionHandler(int id)
{
	//Destruye la bala cuando colisiona con algo
	if (!hit) {
		std::cout << "--> SOY UNA BALA Y HE COLISIONADO <--" << std::endl;
		hit = true;
		physicRB->clearForces();
		//gameObject->Destroy();
	}
}

void WeaponBullet::Update()
{
	if(!hit)
		physicRB->applyCentralImpulse(btVector3(direccion.x*speed, direccion.y*speed, direccion.z*speed));
	else //Se posria hacer aqui un contador para que desapareciese la bala
	{
		
	}
}
