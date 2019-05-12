#include "Enemy.h"
#include "EnemyRigidBody.h"

Enemy::~Enemy()
{
}

void Enemy::LoadFromFile(json obj)
{
}

void Enemy::Start()
{
	currRoom = RoomManager::getInstance()->GetActiveRoom();
	if (currRoom == nullptr) {
#ifdef C_DEBUG
		cout << "ERROR: El enemigo no tiene asignada una sala" << endl;
#endif
	}
	currRoom->AddEnemy();

	player = scene->getGameObject("Player");
	if (player == nullptr) {
#ifdef C_DEBUG
		cout << "ERROR: No se ha encontrado el player en " << gameObject->getName() << endl;
#endif
	}

	playerController = player->getComponent<PlayerController>();

	rbComp = gameObject->getComponent<EnemyRigidBody>();
	rb = rbComp->getRB();

	rb->setGravity(btVector3(0, gravity, 0));

	alive = true;
}


void Enemy::Update()
{
}

void Enemy::OnDeath()
{
	//Crea un corazon al matar al enemigo
	int heartRand = rand() % 100;
	if (heartProb > heartRand) {
		scene->Instantiate("Heart", gameObject->getPosition(), 0.1f);
	}
	
	currRoom->RemoveEnemies();
	gameObject->Destroy();
}

btQuaternion Enemy::VecToQuat(Vector3 vec)
{
	float angle = atan2(vec.x, vec.z);
	btQuaternion q;
	q.setX(0);
	q.setY(1 * sin(angle / 2));
	q.setZ(0);
	q.setW(cos(angle / 2));
	return q;
}

btQuaternion Enemy::VecToQuat(btVector3 vec)
{
	float angle = atan2(vec.x(), vec.z());
	btQuaternion q;
	q.setX(0);
	q.setY(1 * sin(angle / 2));
	q.setZ(0);
	q.setW(cos(angle / 2));
	return q;
}

void Enemy::OnHit()
{
	if (alive) {
		// Crea las particulas
		scene->Instantiate("DeathPS", gameObject->getGlobalPosition(), 0.1f);
		HP--;
		cout << HP << endl;
		if (HP <= 0) {
			alive = false;
			OnDeath();
			rbComp->SetEnabled(false);
		}
	}
}

