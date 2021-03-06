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

	direccion = scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z;
	Vector3 auxVecFinal = Quaternion(Degree(0), Vector3::UNIT_X) * direccion;
	auxVecFinal = Quaternion(Degree(-90), Vector3::UNIT_Y) * auxVecFinal;
	float angle = atan2(auxVecFinal.x, auxVecFinal.z);
	btQuaternion q;
	q.setX(0);
	q.setY(1 * sin(angle / 2));
	q.setZ(0);
	q.setW(cos(angle / 2));
	btRigidBody* rb = gameObject->getComponent<RigidBodyComponent>()->getRB();
	rb->getWorldTransform().setRotation(q);
	playerController = scene->getGameObject("Player")->getComponent<PlayerController>();
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
	//Destruye la bala cuando colisiona con algo que no sea el jugador
	if (!hit && id != PlayerID && id != BulletID && id != HeartID && id != collisionID::ShotgunBulletID) {
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void WeaponBullet::Update()
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
		physicRB->activate(true);
		physicRB->setLinearVelocity(btVector3(direccion.x*finalSpeed, direccion.y*finalSpeed, direccion.z*finalSpeed));
	}
}
