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
	gameObject->setScale(scale);

}

void KnifeGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	scale = obj["scale"];
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];
	Enemy::alive = true;
	HP = obj["HP"];
	heartProb = obj["heartProb"];
}

void KnifeGuy::Update()
{
	if (estado == state::ALIVE) {
		rb->activate();
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		auxVec.normalise(); 
		auxVec *= (moveSpeed * playerController->getGameSpeed());
		meshRend->AnimationSpeed(playerController->getGameSpeed());

		rb->getWorldTransform().setRotation(VecToQuat(auxVec));
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
	playSound("PocholoShout", false, 1);
}

void KnifeGuy::Spawn()
{
}
