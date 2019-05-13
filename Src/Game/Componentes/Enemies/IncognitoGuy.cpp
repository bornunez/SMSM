#include "IncognitoGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

IncognitoGuy::~IncognitoGuy()
{
}

void IncognitoGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();

	meshRend->InitAnimations();

	meshRend->PlayAnimation("Idle", true);
	meshRend->AnimationSpeed(2);

	gameObject->setScale(scale);
	
	tm = TimeManager::getInstance();

	rng = default_random_engine{};

	for (int i = 0; i<posVec.size(); i++) {
		xVec.push_back(posVec[i]);
		zVec.push_back(posVec[i]);
		xVec.push_back(-(float)posVec[i]);
		zVec.push_back(-(float)posVec[i]);
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
	posVec = obj["posVec"];

	shootDist = obj["shootDist"];
	shootTime = obj["shootTime"];

	Enemy::alive = true;
	HP = obj["HP"];
	heartProb = obj["heartProb"];
}


void IncognitoGuy::Update()
{
	if (estado != state::DEAD) {
		rb->activate();
		//Calculo orientacion
		Ogre::Vector3 dir = player->getPosition() - gameObject->getPosition();
		float absDist = abs(dir.x) + abs(dir.z);
		if (estado == state::IDLE) {
			tpTimer += tm->getDeltaTime();
			if (hasTeleported && absDist < shootDist) {
				estado = state::AIMING;
				shootTimer = 0;
			}
			else if (!hasSpawnedPS && tpTimer >= tpTime - 0.2f) {
				// Generar particulas
				scene->Instantiate("PoofPS", gameObject->getPosition(), 0.025f);
				hasSpawnedPS = true;
			}
			else if (tpTimer >= tpTime) {
				rb->clearForces();
				Teleport();
				hasSpawnedPS = false;
				hasTeleported = true;
				tpTimer = 0;
			}		
		}
		else if (estado == state::AIMING) {
			//Dispara despues de apuntar cierto tiempo
			shootTimer += tm->getDeltaTime();
			if (shootTimer >= shootTime) {
				Shoot();
				shootTimer = 0;
				estado = state::IDLE;
				hasTeleported = false;
			}
		}
		//Asignar orientacion
		rb->getWorldTransform().setRotation(VecToQuat(dir));
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
	Enemy::Update();
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
	if (posIndex >= xVec.size()) {
		RandomizeVecs();
		posIndex = 0;
	}

	// Obtenemos transform actual
	btTransform curT;
	rb->getMotionState()->getWorldTransform(curT);

	// Sumamos desplazamiento y aplicamos transform
	btVector3 newOrigin = btVector3{xVec[posIndex], 0, zVec[posIndex]};
	newOrigin += curT.getOrigin();
	curT.setOrigin(newOrigin);
	rb->setWorldTransform(curT);


	rb->setLinearVelocity({ 0, 0, 0 });
	rb->setAngularVelocity({ 0, 0, 0 });


	// Generar particulas
	scene->Instantiate("PoofPS", { newOrigin.x(), newOrigin.y(), newOrigin.z() }, 0.025f);
	posIndex++;
}

void IncognitoGuy::RandomizeVecs()
{
	shuffle(begin(xVec), end(xVec), rng);
	shuffle(begin(zVec), end(zVec), rng);
}

void IncognitoGuy::Shoot()
{
	scene->Instantiate("IncognitoBullet", gameObject->getPosition(), 1);
}