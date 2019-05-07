#include "EnemyBullet.h"


EnemyBullet::EnemyBullet(GameObject* obj):RigidBodyComponent(obj)
{

}


EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Start()
{
	RigidBodyComponent::Start();
	physicRB->setGravity(btVector3(0, grav, 0));
	physicRB->setDamping(linDamp, angDamp);
	physicRB->applyTorqueImpulse(btVector3(0, 0.1, 0));
	direccion = scene->getGameObject("Player")->getNode()->getPosition() - gameObject->getPosition();
	direccion.normalise();
	playerController = scene->getGameObject("Player")->getComponent<PlayerController>();
}

void EnemyBullet::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);

	//Params from file
	grav = obj["gravity"];
	linDamp = obj["linDamp"];
	angDamp = obj["angDamp"];
	speed = obj["speed"];
	recoilTime = obj["recoilTime"];
	ownerID = obj["ownerID"];
}

void EnemyBullet::collisionHandler(int id)
{
	//Destruye la bala cuando colisiona con el jugador
	if (!hit && id != ownerID) {
		hit = true;
		physicRB->clearForces();
		gameObject->Destroy();
	}
}

void EnemyBullet::Update()
{
//	cout << gameObject->getPosition().x << " " << gameObject->getPosition().z << endl;
	if (!hit) 
	{
		float finalSpeed = speed * playerController->getGameSpeed();
		physicRB->setLinearVelocity(btVector3(direccion.x*finalSpeed, direccion.y*finalSpeed / 2, direccion.z*finalSpeed));
	}
	else //Se posria hacer aqui un contador para que desapareciese la bala
	{
		
	}
}
