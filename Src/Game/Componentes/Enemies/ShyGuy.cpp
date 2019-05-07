#include "ShyGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

ShyGuy::~ShyGuy()
{
}

void ShyGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();
	meshRend->InitAnimations();
	meshRend->PlayAnimation("Move", true);
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
	Enemy::alive = true;
}


void ShyGuy::Update()
{
	if (estado != state::DEAD) {
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
		float angle = atan2(auxVec.x, auxVec.z);
		btQuaternion q;
		q.setX(0);
		q.setY(1 * sin(angle / 2));
		q.setZ(0);
		q.setW(cos(angle / 2));

		rb->getWorldTransform().setRotation(q);

		meshRend->AnimationSpeed(playerController->getGameSpeed());
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
}

void ShyGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
}

void ShyGuy::Spawn()
{
}
