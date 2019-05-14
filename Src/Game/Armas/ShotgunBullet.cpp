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
	//direccion = scene->getGameObject("Escopeta")->getGlobalPosition;
	Weapon* esco = scene->getGameObject("Escopeta")->getComponent<Weapon>();
	Vector3 auxVec = scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z* esco->getOffset().z;

	//Vector3 auxVec = gameObject->getParent()->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	//Vector3 auxVec2 = gameObject->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	//float angle2 = atan(auxVec2.y / auxVec2.z)* -57.2958;
	Vector3 auxVecFinal = Quaternion(Degree(esco->getOffset().x), Vector3::UNIT_Y) * auxVec;//scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z;
	/*direccion = gameObject->getGlobalPosition() - ((esco->getGameObject()->getParent()->getGlobalPosition() + auxVecFinal + Vector3(0, 0.185, 0)) -
		(scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z)*(0.00075/esco->getDispersion()));
	direccion.normalise();*/

	Vector3 pos = gameObject->getGlobalPosition();
	Vector3 pos2 = pos+ auxVec;
	Vector3 auxOffImp;
	Vector3 auxVecNorm = auxVec;
	auxVecNorm.normalise();
	//VARIABLE MAGICA 1.34 Y LA INVERSA 0.74 NO TOCAR
	if((auxVecNorm.x < 0 &&Math::Abs(auxVecNorm.x) >= Math::Abs(auxVecNorm.z)*1.34) || (-auxVecNorm.z < 0 && Math::Abs(auxVecNorm.z) >= Math::Abs(auxVecNorm.x)*0.74))
		auxOffImp = Vector3(0, esco->getOffset().y - 0.004, 0);
	else 
		auxOffImp = Vector3(0, esco->getOffset().y+0.003, 0);
	pos2 += (pos - (esco->getGameObject()->getParent()->getGlobalPosition() + auxVecFinal + auxOffImp))*10;
	direccion = pos2 - pos;
	playerController = scene->getGameObject("Player")->getComponent<PlayerController>();
	/*direccion = auxVec;
	direccion.normalise();
	nuevaDir = gameObject->getGlobalPosition() - (esco->getGameObject()->getParent()->getGlobalPosition() + auxVecFinal + Vector3(0, esco->getOffset().y, 0));
	nuevaDir.normalise();
	*/
	/*direccion = scene->getGame()->getViewport()->getCamera()->getRealOrientation()* Vector3::NEGATIVE_UNIT_Z;
	direccion.normalise();
	Vector3 auxVecFinal = Quaternion(Degree(esco->getOffset().x), Vector3::UNIT_Y) * auxVec;
	nuevaDir = gameObject->getGlobalPosition()- (esco->getGameObject()->getParent()->getGlobalPosition() + auxVecFinal + Vector3(0, 0.18, 0));
	nuevaDir.normalise();*/
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

