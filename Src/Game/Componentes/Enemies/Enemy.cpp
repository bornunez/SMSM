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

	//Find mesh renderer
	std::list<Component*> comps = gameObject->getComponents();
	bool found = false;
	auto it = comps.begin();
	while (!found && it != comps.end())
	{
		EnemyRigidBody* c = dynamic_cast<EnemyRigidBody*>(*it);
		if (c != nullptr) {
			found = true;
			rbComp = c;
		}
		it++;
	}
	rb = rbComp->getRB();
}


void Enemy::Update()
{
}

void Enemy::OnDeath()
{
	scene->Instantiate("DeathPS",gameObject->getGlobalPosition(),0.1f);
	currRoom->RemoveEnemies();
	gameObject->Destroy();
}

