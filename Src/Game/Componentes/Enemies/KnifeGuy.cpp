#include "KnifeGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

KnifeGuy::~KnifeGuy()
{
}

void KnifeGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();
	meshRend->InitAnimations();

	meshRend->PlayAnimation("Move", true);
	meshRend->AnimationSpeed(2);
	rb->setGravity(btVector3(0, gravity, 0));
}

void KnifeGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];
	Enemy::alive = true;
}


void KnifeGuy::Update()
{
	if (estado == state::ALIVE) {
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		auxVec.normalise(); 
		auxVec *= (moveSpeed * playerController->getGameSpeed());
		meshRend->AnimationSpeed(playerController->getGameSpeed());

		float angle = atan2(auxVec.x, auxVec.z);
		btQuaternion q;
		q.setX(0);
		q.setY(1 * sin(angle / 2));
		q.setZ(0);
		q.setW(cos(angle / 2));

		rb->getWorldTransform().setRotation(q);
		rb->setLinearVelocity({auxVec.x, 0, auxVec.z});
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (estado == state::DEAD && meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
}

void KnifeGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->AnimationSpeed(2);
}

void KnifeGuy::Spawn()
{
}
