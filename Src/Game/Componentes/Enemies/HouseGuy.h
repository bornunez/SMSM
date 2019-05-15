#pragma once
#include "Enemy.h"
#include "../../Src/MotorEngine/TimeManager.h"

class MeshRenderer;
class HouseGuy :
	public Enemy
{
private:
	enum state{IDLE, SPAWNING, DEAD};
	state estado = state::IDLE;
	MeshRenderer* meshRend = nullptr;
	float scale = 0;
	float speedTime = 0;
	float speedTimer = 0;
	float spawnTime = 0;
	float spawnTimer = 0;
	float spawnDelay = 0;
	float spawnDistance = 0;
	int onDeathSpawns = 0;
	int maxSpawns = 0;
	int spawnCount = 0;
	Ogre::Vector3 velVec = { 0,0,0 };

	float defAnimSp = 0;
	float deathAnimSp = 0;

	TimeManager* tm = nullptr;

	void Start();
	void LoadFromFile(json obj);
	void SpawnEnemy(Vector3 pos);

protected:
	virtual void Update();
	virtual void OnDeath();

public:
	HouseGuy(GameObject* obj) : Enemy(obj) {};
	~HouseGuy();
};

