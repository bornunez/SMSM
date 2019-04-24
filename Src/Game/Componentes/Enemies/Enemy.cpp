#include "Enemy.h"
#include "EnemyRigidBody.h"


Enemy::~Enemy()
{
}

void Enemy::Start()
{
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

