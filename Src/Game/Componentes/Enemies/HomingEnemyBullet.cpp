#include "HomingEnemyBullet.h"


HomingEnemyBullet::HomingEnemyBullet(GameObject* obj):RigidBodyComponent(obj)
{

}


HomingEnemyBullet::~HomingEnemyBullet()
{
}

void HomingEnemyBullet::Start()
{
	RigidBodyComponent::Start();
	physicRB->setGravity(btVector3(0, grav, 0));
	physicRB->setDamping(linDamp, angDamp);
	physicRB->applyTorqueImpulse(btVector3(0, 0.1, 0));
	direccion = scene->getGameObject("Player")->getNode()->getPosition() - gameObject->getPosition();
	direccion.normalise();
	playerController = scene->getGameObject("Player")->getComponent<PlayerController>();

	gameObject->setScale(scale);
}

void HomingEnemyBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Params from file
	grav = obj["gravity"];
	linDamp = obj["linDamp"];
	angDamp = obj["angDamp"];
	speed = obj["speed"];
	recoilTime = obj["recoilTime"];
	ownerID = obj["ownerID"];
	deathTime = obj["deathTime"];
	scale = obj["scale"];
}

void HomingEnemyBullet::collisionHandler(int id)
{
	//Destruye la bala cuando colisiona con el jugador
	if (!hit && id != ownerID) {
		scene->Instantiate("LitleBoomPS", gameObject->getPosition(), 0.025f);
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void HomingEnemyBullet::Update()
{
	if (!hit) 
	{
		if (deathTimer < deathTime) 
			deathTimer += TimeManager::getInstance()->getDeltaTime()*playerController->getGameSpeed();
		else
		{
			physicRB->clearForces();
			gameObject->Destroy();
		}
		direccion = scene->getGameObject("Player")->getNode()->getPosition() - gameObject->getPosition();
		direccion.normalise();
		float finalSpeed = speed * playerController->getGameSpeed();
		physicRB->setLinearVelocity(btVector3(direccion.x*finalSpeed, direccion.y*finalSpeed, direccion.z*finalSpeed));
	}
}
