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
	Weapon* esco = scene->getGameObject("Escopeta")->getComponent<Weapon>();
	Vector3 auxVec = scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z* esco->getOffset().z;

	Vector3 auxVecFinal = Quaternion(Degree(esco->getOffset().x), Vector3::UNIT_Y) * auxVec;

	Vector3 pos = gameObject->getGlobalPosition();
	Vector3 pos2 = pos+ auxVec;
	Vector3 auxOffImp;
	Vector3 auxVecNorm = auxVec;
	auxVecNorm.normalise();

	if((auxVecNorm.x < 0 &&Math::Abs(auxVecNorm.x) >= Math::Abs(auxVecNorm.z) * INVERSION_ROT_MAX) || (-auxVecNorm.z < 0 && Math::Abs(auxVecNorm.z) >= Math::Abs(auxVecNorm.x) * INVERSION_ROT_MIN))
		auxOffImp = Vector3(0, esco->getOffset().y - 0.004, 0);
	else 
		auxOffImp = Vector3(0, esco->getOffset().y+0.003, 0);

	pos2 += (pos - (esco->getGameObject()->getParent()->getGlobalPosition() + auxVecFinal + auxOffImp)) * 10;
	direccion = pos2 - pos;
	playerController = scene->getGameObject("Player")->getComponent<PlayerController>();
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
	if (!hit && id != PlayerID && id != BulletID && id != HeartID  && id != collisionID::ShotgunBulletID) {
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void ShotgunBullet::Update()
{
	if (!hit)
	{
		if (actDeathTime < deathTime) actDeathTime += TimeManager::getInstance()->getDeltaTime()*playerController->getGameSpeed();
		else
		{
			physicRB->clearForces();
			gameObject->Destroy();
		}
		float finalSpeed = speed * playerController->getGameSpeed();
		physicRB->setLinearVelocity(btVector3(direccion.x*finalSpeed, direccion.y*finalSpeed, direccion.z*finalSpeed));
	}
}