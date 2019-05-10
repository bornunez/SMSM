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
	gameObject->setScale(scale);

	tm = TimeManager::getInstance();
}

void BombGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	gravity = obj["gravity"];
	scale = obj["scale"];
	moveSpeed = obj["moveSpeed"];
	chaseDist = obj["chaseDist"];
	expDist = obj["expDist"];
	expRadius = obj["expRadius"];
	expTime = obj["expTime"];
	Enemy::alive = true;
}


void BombGuy::Update()
{
	if (estado != state::DEAD) {
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		if (estado == state::IDLE) {
			if (absDist < chaseDist) {
				estado = state::CHASING;
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
				//meshRend->PlayAnimation("EXPLOTIDO");
			}
		}
		else if (estado == state::EXPLODING) {
			expTimer += tm->getDeltaTime();
			if (expTimer >= expTime) {
				Explode();
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

void BombGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
}

void BombGuy::Spawn()
{
}

void BombGuy::Explode()
{
	//crear particulas
	scene->Instantiate("PoofPS", gameObject->getPosition(), 0.025f);
	//Crear explosion
	HP = 0;
	Enemy::OnHit();
}
