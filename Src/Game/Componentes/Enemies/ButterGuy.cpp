#include "ButterGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

ButterGuy::~ButterGuy()
{
}

void ButterGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();
	meshRend->InitAnimations();
	meshRend->PlayAnimation("Move", true);
	meshRend->SetAnimationSpeed(defAnimSp * playerController->getGameSpeed());

	gameObject->setScale(scale);

	tm = TimeManager::getInstance();
}

void ButterGuy::LoadFromFile(json obj)
{
	//Params from file
	gravity = obj["gravity"];
	scale = obj["scale"];
	moveSpeed = obj["moveSpeed"];
	dist = obj["dist"];
	distFactor = obj["distFactor"];
	shootTime = obj["shootTime"];
	Enemy::alive = true;
	HP = obj["HP"];
	heartProb = obj["heartProb"];
	defAnimSp = obj["defAnimSp"];
	deathAnimSp = obj["deathAnimSp"];
}


void ButterGuy::Update()
{
	if (estado != state::DEAD) {
		rb->activate();
		rb->setAngularVelocity(btVector3(0, 0, 0));
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		//No esta tan cerca como para tener que alejarse
		if (estado == state::IDLE) {
			//Si esta demasiado lejos se acerca hasta entrar en el rango de disparo
			if (absDist > dist*distFactor) {
				auxVec.normalise();
				auxVec *= (moveSpeed * playerController->getGameSpeed());
				rb->setLinearVelocity({ auxVec.x, 0, auxVec.z});
			}
			//Si esta demasiado cerca no puede disparar, pasa a huir
			else if (absDist < dist) {
				estado = state::FLEEING;
				rb->setLinearVelocity(btVector3(0, 0, 0));
			}
			//Si esta en rango de disparo
			else {
				estado = state::AIMING;
				rb->setLinearVelocity(btVector3(0, 0, 0));
				shootTimer = 0;
			}
		}
		//si esta huyendo...
		else if (estado == state::FLEEING) {
			auxVec = -auxVec;
			auxVec.normalise();
			auxVec *= (moveSpeed * playerController->getGameSpeed());
			rb->setLinearVelocity({ auxVec.x, 0, auxVec.z});
			if (absDist > dist) {
				estado = state::IDLE;
				rb->setLinearVelocity(btVector3(0, 0, 0));
			}
		}
		//Si esta apuntando
		else if (estado == state::AIMING) {
			//Dispara cada cierto tiempo
			//Crear bala con direccion auxvec
			shootTimer += tm->getDeltaTime() * playerController->getGameSpeed();
			if (shootTimer >= shootTime) {
				Shoot();
				shootTimer = 0;
			}
			//Si deja de estar en rango vuelve a estar idle
			if (absDist < dist || absDist > dist*distFactor) {
				estado = state::IDLE;
			}
		}
		//Mira al jugador
		if (!playerController->isTimeStopped()) rb->getWorldTransform().setRotation(VecToQuat(auxVec));
		meshRend->SetAnimationSpeed(defAnimSp * playerController->getGameSpeed());
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

void ButterGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->SetAnimationSpeed(deathAnimSp * playerController->getGameSpeed());
	playSound("ButterShout", false, 1);
}

void ButterGuy::Shoot()
{
	scene->Instantiate("ButterBullet", gameObject->getPosition(), 1);
}
