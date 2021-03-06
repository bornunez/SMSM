#pragma once
#include "Enemy.h"
#include <vector>
#include <algorithm>
#include <random>
#include "../../Src/MotorEngine/TimeManager.h"

class MeshRenderer;
class BossGuy :
	public Enemy
{
private:
	enum state{IDLE, AIMING, MULTI_AIMING, SPAWNING, DEAD};
	enum action{AIM, MULTI_AIM, SPAWN};
	string bulletType = "";
	state estado = state::IDLE;
	action nextAction = action::AIM;
	MeshRenderer* meshRend = nullptr;
	float scale = 0;
	default_random_engine rng;

	std::vector<string> spawnables;
	int spawnIndex = 0;

	std::vector<string> actions;
	int actionIndex = 0;

	json enemiesVec;
	json actionsVec;

	float spawnDelay = 0;
	float spawnTime = 0;

	float multiBulletDelay = 0;
	float multiShootDelay = 0;
	int multiShootBullets = 0;
	float multiShootTime = 0;
	float shootTime = 0;
	int multiShootIndex = 0;

	float timer = 0;

	float defAnimSp = 0;
	float homingAnimSp = 0;
	float deathAnimSp = 0;
	float multiAnimSp = 0;
	float currAnimSp = 0;
	float auxAnimSp = 0;

	TimeManager* tm = nullptr;

	void Start();
	void LoadFromFile(json obj);
	void Shoot();
	void SpawnEnemy(Vector3 pos);
	void UpdateNextAction();
	void SetNextAction(string action);
	void ActionEnd();

protected:
	virtual void Update();
	virtual void OnDeath();

public:
	BossGuy(GameObject* obj) : Enemy(obj) {};
	~BossGuy();
};

