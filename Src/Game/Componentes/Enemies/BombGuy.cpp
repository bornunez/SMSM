#include "BombGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

BombGuy::~BombGuy()
{
}

void BombGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();
	meshRend->InitAnimations();
	meshRend->PlayAnimation("Move", true);
	meshRend->SetAnimationSpeed(defAnimSp * playerController->getGameSpeed());
	gameObject->setScale(scale);

	tm = TimeManager::getInstance();
}

void BombGuy::LoadFromFile(json obj)
{
	//Params from file
	gravity = obj["gravity"];
	scale = obj["scale"];
	moveSpeed = obj["moveSpeed"];
	chaseDist = obj["chaseDist"];
	expDist = obj["expDist"];
	expRadius = obj["expRadius"];
	expTime = obj["expTime"];
	Enemy::alive = true;
	HP = obj["HP"];
	heartProb = obj["heartProb"];
	defAnimSp = obj["defAnimSp"];
	deathAnimSp = obj["deathAnimSp"];
}


void BombGuy::Update()
{
	if (estado != state::DEAD) {
		rb->activate();
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		if (estado == state::IDLE) {
			if (absDist < chaseDist) {
				estado = state::CHASING;
				meshRend->PlayAnimation("Move", true);
			}
		}
		else if (estado == state::CHASING) {
			auxVec.normalise();
			auxVec *= (moveSpeed * playerController->getGameSpeed());
			rb->setLinearVelocity({ auxVec.x , 0, auxVec.z});
			if (absDist > chaseDist) {
				rb->setLinearVelocity(btVector3(0, 0, 0));
				estado = state::IDLE;
				meshRend->StopAnimation(true);
			}
			else if (absDist < expDist) {
				rb->setLinearVelocity(btVector3(0, 0, 0));
				estado = state::EXPLODING;
				meshRend->StopAnimation(true);
				meshRend->PlayAnimation("Explode", true);
			}
		}
		else if (estado == state::EXPLODING) {
			expTimer += tm->getDeltaTime() * playerController->getGameSpeed();
			if (expTimer >= expTime) {
				OnDeath();
			}
		}

		meshRend->SetAnimationSpeed(defAnimSp * playerController->getGameSpeed());
		if(!playerController->isTimeStopped()) rb->getWorldTransform().setRotation(VecToQuat(auxVec));
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else {
		meshRend->SetAnimationSpeed(deathAnimSp * playerController->getGameSpeed());
		if (meshRend->AnimationHasEnded("Death")) {
			Enemy::OnDeath();
		}
	}
	Enemy::Update();
}

void BombGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->SetAnimationSpeed(deathAnimSp * playerController->getGameSpeed());
	Explode();
}

void BombGuy::Explode()
{
	//crear particulas
	scene->Instantiate("BoomPS", gameObject->getPosition(), 0.025f);
	//Calcular explosion
	Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
	float absDist = abs(auxVec.x) + abs(auxVec.z);
	if (absDist < expRadius) {
		//Enviar da�o a jugador
		playerController->receiveDamage();
	}
	//Sonido explosion
	playSound("BombExplode", false, 1);
}
