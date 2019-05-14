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
	enum state{IDLE, AIMING, SPAWNING, DEAD};
	enum action{AIM, SPAWN};
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

	float shootTime = 0;

	float timer = 0;

	float defAnimSp = 0;
	float homingAnimSp = 0;
	float deathAnimSp = 0;
	float currAnimSp = 0;

	TimeManager* tm = nullptr;

public:
	void Start();
	BossGuy(GameObject* obj) : Enemy(obj) {};
	~BossGuy();
	void LoadFromFile(json obj);
	virtual void Update();
	virtual void OnDeath();
	virtual void Spawn();
	void Shoot();
	void SpawnEnemy(Vector3 pos);
	void UpdateNextAction();
	void SetNextAction(string action);
	void ActionEnd();


};

