#include "Enemy.h"
#include "EnemyRigidBody.h"

Enemy::~Enemy()
{
}

void Enemy::Start()
{
	currRoom = RoomManager::getInstance()->GetActiveRoom();
	if (currRoom == nullptr)
		cout << "ERROR: El enemigo no tiene asignada una sala" << endl;
	currRoom->AddEnemy();

	player = scene->getGameObject("Player");
	if(player==nullptr) cout << "ERROR: No se ha encontrado el player en " << gameObject->getName() << endl;

	playerController = player->getComponent<PlayerController>();

	rbComp = gameObject->getComponent<EnemyRigidBody>();
	rb = rbComp->getRB();
	alive = true;
}


void Enemy::Update()
{
}

void Enemy::OnDeath()
{
	currRoom->RemoveEnemies();
	gameObject->Destroy();
}

void Enemy::OnHit()
{
	if (alive) {
		// Crea las particulas
		scene->Instantiate("DeathPS", gameObject->getGlobalPosition(), 0.1f);
		HP--;
		if (HP <= 0) {
			alive = false;
			OnDeath();
			rbComp->SetEnabled(false);
		}
	}
}

