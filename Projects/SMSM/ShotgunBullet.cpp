#include "ShotgunBullet.h"
#include "../../Src/Game/Armas/Weapon.h"


ShotgunBullet::ShotgunBullet(GameObject* obj) :RigidBodyComponent(obj)
{

}


ShotgunBullet::~ShotgunBullet()
{
}

void ShotgunBullet::Start()
{
	RigidBodyComponent::Start();
	physicRB->setGravity(btVector3(0, grav, 0));
	physicRB->setDamping(linDamp, angDamp);
	//physicRB->applyTorqueImpulse(btVector3(0, 0.1, 0));
	direccion = scene->getGameObject("Escopeta")->getGlobalPosition;

	Vector3 auxVec = scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z*
		scene->getGameObject("Escopeta")->getComponent<Weapon>();
	//Vector3 auxVec = gameObject->getParent()->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	//Vector3 auxVec2 = gameObject->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	//float angle2 = atan(auxVec2.y / auxVec2.z)* -57.2958;
	Vector3 auxVecFinal = Quaternion(Degree(offset.x), Vector3::UNIT_Y) * auxVec;//scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z;

}

void ShotgunBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Params from file
	grav = obj["gravity"];
	linDamp = obj["linDamp"];
	angDamp = obj["angDamp"];
	speed = obj["speed"];
	recoilTime = obj["recoilTime"];
}

void ShotgunBullet::collisionHandler(int id)
{
	//Destruye la bala cuando colisiona con algo que no sea el jugador
	if (!hit && id != 0 && id != 1) {
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void ShotgunBullet::Update()
{
	if (!hit)
	{
		physicRB->applyCentralImpulse(btVector3(direccion.x*speed, direccion.y*speed, direccion.z*speed));
		//physicRB->clearForces();
	}
	else //Se posria hacer aqui un contador para que desapareciese la bala
	{

	}
}

