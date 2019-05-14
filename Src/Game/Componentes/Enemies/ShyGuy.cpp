#include "ShyGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"
#include "../../Src/MotorEngine/AudioManager.h"

ShyGuy::~ShyGuy()
{
}

void ShyGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();
	meshRend->InitAnimations();
	meshRend->PlayAnimation("Move", true);
	meshRend->SetAnimationSpeed(defAnimSp * playerController->getGameSpeed());
	gameObject->setScale(scale);
}

void ShyGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	gravity = obj["gravity"];
	scale = obj["scale"];
	moveSpeed = obj["moveSpeed"];
	dist = obj["dist"];
	distFactor = obj["distFactor"];
	maxFactor = obj["maxFactor"];
	minFactor = obj["maxFactor"];
	defAnimSp = obj["defAnimSp"];
	deathAnimSp = obj["deathAnimSp"];
	Enemy::alive = true;
	HP = obj["HP"];
}


void ShyGuy::Update()
{
	if (estado != state::DEAD) {
		rb->activate();

		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		if (estado == state::IDLE) {
			if (absDist > dist*distFactor) {
				auxVec *= playerController->getGameSpeed();
				rb->setLinearVelocity({ auxVec.x / 4, 0, auxVec.z / 4 });
			}
			else if (absDist < dist) {
				meshRend->PlayAnimation("Move", true);
				rb->setLinearVelocity(btVector3(0, 0, 0));
				estado = state::FLEEING;
			}
		}
		else if (estado == state::FLEEING) {		
			auxVec.normalise();
			auxVec *= -(moveSpeed * playerController->getGameSpeed());

			rb->setLinearVelocity({ auxVec.x * ((rand() % maxFactor + minFactor)/100) , 0, auxVec.z * ((rand() % maxFactor + minFactor) / 100) });
			if (absDist > dist) {
				rb->setLinearVelocity(btVector3(0, 0, 0));
				estado = state::IDLE;
				meshRend->StopAnimation(true);
			}
		}

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

void ShyGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->SetAnimationSpeed(deathAnimSp * playerController->getGameSpeed());
	playSound("ShyGuyShout", false, 1);

}

void ShyGuy::Spawn()
{
}
