#include "HouseGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

HouseGuy::~HouseGuy()
{
}

void HouseGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();

	meshRend->InitAnimations();

	meshRend->PlayAnimation("Move", true);
	meshRend->AnimationSpeed(2);

	gameObject->setScale(scale);
	
	tm = TimeManager::getInstance();
}
void HouseGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	scale = obj["scale"];
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];
	speedTime = obj["speedTime"];
	spawnTime = obj["spawnTime"];
	spawnDelay = obj["spawnDelay"];
	spawnDistance = obj["spawnDistance"];
	onDeathSpawns = obj["onDeathSpawns"];
	Enemy::alive = true;
}


void HouseGuy::Update()
{
	if (estado != state::DEAD) {
		spawnTimer += tm->getDeltaTime();
		if (estado == state::IDLE) {
			speedTimer += tm->getDeltaTime();
			if (speedTimer >= speedTime) {
				//Calculo velocidad
				velVec = Ogre::Vector3(((rand() % 200 - 100)), 0 , ((rand() % 200 - 100)));
				velVec.normalise();
				velVec *=moveSpeed;

				//Calculo orientacion
				float angle = atan2(velVec.x, velVec.z);
				btQuaternion q;
				q.setX(0);
				q.setY(1 * sin(angle / 2));
				q.setZ(0);
				q.setW(cos(angle / 2));
				//Asignar orientacion
				rb->clearForces();
				rb->getWorldTransform().setRotation(q);

				speedTimer = 0;
			}
			//Asignar velocidad
			rb->setLinearVelocity({ velVec.x, 0, velVec.z});

			//Gestion spawntimer
			if (spawnTimer >= spawnTime) {
				estado = state::SPAWNING;
				//Calculo orientacion
				velVec = player->getPosition() - gameObject->getPosition();
				velVec.normalise();
				//Asignar orientacion
				rb->getWorldTransform().setRotation(VecToQuat(velVec));
				rb->setLinearVelocity({ 0, 0, 0 });
			}
			
		}
		else if (estado == state::SPAWNING) {
			if (spawnTimer >= spawnTime + spawnDelay) {
				SpawnEnemy(gameObject->getPosition() + velVec * spawnDistance);
				spawnTimer = 0;
				estado = state::IDLE;
				speedTimer = speedTime;
			}
		}

	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		for (int i = 0; i < onDeathSpawns; i++) {
			SpawnEnemy(gameObject->getPosition());
		}
		Enemy::OnDeath();
	}
}

void HouseGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->AnimationSpeed(2);
}

void HouseGuy::Spawn()
{
}

void HouseGuy::SpawnEnemy(Vector3 pos)
{
	scene->Instantiate("PoofPS", pos, 0.025f);
	scene->Instantiate("ShyGuy", pos, 0.5f);
	RoomManager::getInstance()->GetActiveRoom()->AddEnemy();
}
