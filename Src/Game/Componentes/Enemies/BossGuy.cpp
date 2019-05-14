#include "BossGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

BossGuy::~BossGuy()
{
}

void BossGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();

	meshRend->InitAnimations();

	auxAnimSp = defAnimSp;
	currAnimSp = defAnimSp;
	meshRend->PlayAnimation("Move", true);
	meshRend->SetAnimationSpeed(currAnimSp * playerController->getGameSpeed());

	gameObject->setScale(scale);
	
	tm = TimeManager::getInstance();

	rng = default_random_engine{};

	for (int i = 0; i<enemiesVec.size(); i++) {
		spawnables.push_back(enemiesVec[i]);
	}

	for (int i = 0; i < actionsVec.size(); i++) {
		actions.push_back(actionsVec[i]);
	}

	SetNextAction(actions[actionIndex]);
	shuffle(begin(spawnables), end(spawnables), rng);
}
void BossGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	scale = obj["scale"];
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];

	enemiesVec = obj["enemiesVec"];
	actionsVec = obj["actionsVec"];

	spawnDelay = obj["spawnDelay"];
	spawnTime = obj["spawnTime"];

	multiShootBullets = obj["multiShootBullets"];
	multiBulletDelay = obj["multiBulletDelay"];
	multiShootDelay = obj["multiShootDelay"];
	multiShootTime = obj["multiShootTime"];
	shootTime = obj["shootTime"];

	Enemy::alive = true;
	HP = obj["HP"];
	heartProb = obj["heartProb"];
	defAnimSp = obj["defAnimSp"];
	homingAnimSp = obj["homingAnimSp"];
	multiAnimSp = obj["multiAnimSp"];
	deathAnimSp = obj["deathAnimSp"];
}


void BossGuy::Update()
{
	if (estado != state::DEAD) {
		rb->activate();
		//Calculo orientacion
		Ogre::Vector3 dir = player->getPosition() - gameObject->getPosition();
	
		timer += tm->getDeltaTime() * playerController->getGameSpeed();
		if (estado == state::IDLE) {
			dir.normalise();
			dir *= (moveSpeed * playerController->getGameSpeed());
			rb->setLinearVelocity({ dir.x, 0, dir.z });
			if (nextAction == action::AIM) {
				if (timer >= shootTime) {
					estado = state::AIMING;
					meshRend->PlayAnimation("Shoot", false);
					currAnimSp = auxAnimSp;
				}
			}
			else if (nextAction == action::MULTI_AIM) {
				if (timer >= multiShootTime) {
					estado = state::MULTI_AIMING;
					rb->setLinearVelocity({ 0,0,0 });
					meshRend->PlayAnimation("Shoot", false);
					timer = 0;
					currAnimSp = auxAnimSp;
				}
			}
			else if (nextAction == action::SPAWN) {
				if (timer >= spawnTime) {
					rb->setLinearVelocity({ 0,0,0 });
					estado = state::SPAWNING;
					meshRend->PlayAnimation("SpawnEnemies", true);
				}
			}
		}
		else if (estado == state::AIMING) {
			//Dispara despues de apuntar cierto tiempo
			if (meshRend->AnimationHasEnded("Shoot")) {
				Shoot();
				ActionEnd();
				UpdateNextAction();
			}
		}
		else if (estado == state::MULTI_AIMING) {
			//Dispara despues de apuntar cierto tiempo
			if (timer >= multiShootDelay) {
				if (multiShootIndex < multiShootBullets) {
					if (timer >= (multiShootDelay + multiBulletDelay*multiShootIndex/multiShootBullets)) {
						Shoot();
						multiShootIndex++;
					}
				}
				else {
					multiShootIndex = 0;
					ActionEnd();
					UpdateNextAction();
				}
			}
		}
		else if (estado == state::SPAWNING) {
			//Spawnea un enemigo despues cierto tiempo
			if (timer >= spawnTime + spawnDelay) {
				SpawnEnemy(gameObject->getPosition() + Vector3{ 0,0,1 });
				ActionEnd();
				UpdateNextAction();
			}
		}
		//Asignar orientacion
		if (!playerController->isTimeStopped()) rb->getWorldTransform().setRotation(VecToQuat(dir));
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else {
		if (meshRend->AnimationHasEnded("Death")) {
			Enemy::OnDeath();
		}
	}
	meshRend->SetAnimationSpeed(currAnimSp * playerController->getGameSpeed());
	Enemy::Update();
}

void BossGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	currAnimSp = deathAnimSp;
	meshRend->SetAnimationSpeed(currAnimSp * playerController->getGameSpeed());
}

void BossGuy::Spawn()
{
}

void BossGuy::Shoot()
{
	scene->Instantiate(bulletType, gameObject->getPosition(), 1);
}

void BossGuy::SpawnEnemy(Vector3 pos)
{
	scene->Instantiate("LitlePoofPS", pos, 0.025f);
	scene->Instantiate(spawnables[spawnIndex], pos, 0.5f);

	spawnIndex++;
	if (spawnIndex >= spawnables.size()) {
		shuffle(begin(spawnables), end(spawnables), rng);
		spawnIndex = 0;
	}
}


void BossGuy::UpdateNextAction() {
	actionIndex++;
	if (actionIndex >= actions.size()) {
		actionIndex = 0;
	}
	SetNextAction(actions[actionIndex]);
}

void BossGuy::SetNextAction(string action)
{
	if (action == "Shoot") {
		nextAction = action::AIM;
		bulletType = "BossBullet";
	}
	else if (action == "HomingShoot") {
		nextAction = action::AIM;
		bulletType = "HomingEnemyBullet";
		auxAnimSp = homingAnimSp;
	}
	else if (action == "Spawn") {
		nextAction = action::SPAWN;
	}
	else if (action == "MultiShoot") {
		nextAction = action::MULTI_AIM;
		bulletType = "BossBullet";
		auxAnimSp = multiAnimSp;
	}
}

void BossGuy::ActionEnd()
{				
	timer = 0;
	estado = state::IDLE;
	meshRend->PlayAnimation("Move", true);
	currAnimSp = defAnimSp;
	meshRend->SetAnimationSpeed(currAnimSp * playerController->getGameSpeed());
}
