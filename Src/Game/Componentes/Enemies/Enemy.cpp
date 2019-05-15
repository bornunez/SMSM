#include "Enemy.h"
#include "EnemyRigidBody.h"
#include "../../Src/MotorEngine/AudioManager.h"

Enemy::~Enemy()
{
}

void Enemy::LoadFromFile(json obj)
{
}

void Enemy::Start()
{
	if(!started){
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
	started = true;
}


void Enemy::Update()
{
	float dt = TimeManager::getInstance()->getDeltaTime();
	if (currHurtTime > -50) {
		if (currHurtTime - dt  >= 0) {
			currHurtTime -= dt;
		}
		else {
			cout << "Acabo de estar rojo" << endl;
			currHurtTime = -60;
			gameObject->setMaterial("");
		}
	}
}

void Enemy::playSound(string name, bool loop, float volume)
{
	GameObject* playerAux = scene->getGameObject("Player");
	Ogre::Matrix4 A;
	A.makeTransform(gameObject->getGlobalPosition(), Ogre::Vector3::UNIT_SCALE, gameObject->getGlobalOrientation());
	Ogre::Matrix4 B;
	B.makeTransform(player->getGlobalPosition(), Ogre::Vector3::UNIT_SCALE, player->getGlobalOrientation());

	Ogre::Matrix4 A_BSpace = B.inverse() * A;
	Vector3 relPos = A_BSpace.getTrans();
	AudioManager::getInstance()->play3DSound(name, relPos.x, relPos.y, relPos.z, loop, volume, CHANNEL::Enemigos);
};

void Enemy::OnDeath()
{
	playerController->increaseEnemyKillCount();

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

void Enemy::OnHit(int damage)
{
	if (alive) {
		// Crea las particulas
		scene->Instantiate("DeathPS", gameObject->getGlobalPosition(), 0.1f);
		gameObject->setMaterial("Materials/RedHurt");
		currHurtTime = hurtTime;
		

		HP -= damage;
		cout << HP << endl;
		if (HP <= 0) {
			alive = false;
			OnDeath();
			rbComp->SetEnabled(false);
		}
	}
}

