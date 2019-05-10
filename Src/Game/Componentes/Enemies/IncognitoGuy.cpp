#include "IncognitoGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

IncognitoGuy::~IncognitoGuy()
{
}

void IncognitoGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();

	meshRend->InitAnimations();

	meshRend->PlayAnimation("Move", true);
	meshRend->AnimationSpeed(2);

	gameObject->setScale(scale);
	
	tm = TimeManager::getInstance();

	rng = default_random_engine{};

	xVec.resize(1 + tpFactor * 2);
	zVec.resize(1 + tpFactor * 2);
	for (int i = -tpFactor; i++; i <= tpFactor) {
		xVec.push_back(i);
		zVec.push_back(i);
	}
	
	RandomizeVecs();
}
void IncognitoGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	scale = obj["scale"];
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];
	tpTime = obj["tpTime"];
	tpFactor = obj["tpFactor"];

	Enemy::alive = true;
}


void IncognitoGuy::Update()
{
	if (estado != state::DEAD) {
		if (estado == state::IDLE) {
			tpTimer += tm->getDeltaTime();
			if (tpTimer >= tpTime) {
				//Teletransportar

				//Calculo orientacion
				Ogre::Vector3 dir = player->getPosition() - gameObject->getPosition();
				float angle = atan2(dir.x, dir.z);
				btQuaternion q;
				q.setX(0);
				q.setY(1 * sin(angle / 2));
				q.setZ(0);
				q.setW(cos(angle / 2));
				//Asignar orientacion
				rb->getWorldTransform().setRotation(q);
				rb->clearForces();

				Teleport();
				tpTimer = 0;
			}			
		}
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
}

void IncognitoGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->AnimationSpeed(2);
}

void IncognitoGuy::Spawn()
{
}

void IncognitoGuy::Teleport()
{
	// Obtenemos transform actual
	btTransform curT;
	rb->getMotionState()->getWorldTransform(curT);

	// Sumamos desplazamiento y aplicamos transform
	curT.setOrigin(curT.getOrigin() + btVector3{ btScalar(xVec[posIndex]), 0, btScalar(zVec[posIndex])});
	rb->getMotionState()->setWorldTransform(curT);

	// Generar particulas
	scene->Instantiate("PoofPS", gameObject->getPosition(), 0.025f);
	posIndex++;

	if (posIndex > (tpFactor * 2)) {
		RandomizeVecs();
		posIndex = 0;
	}

}

void IncognitoGuy::RandomizeVecs()
{
	shuffle(begin(xVec), end(xVec), rng);
	shuffle(begin(zVec), end(zVec), rng);
}
