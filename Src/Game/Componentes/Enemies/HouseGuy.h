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
	float speedTime = 0;
	float speedTimer = 0;
	float spawnTime = 0;
	float spawnTimer = 0;
	float spawnDelay = 0;
	float spawnDistance = 0;
	Ogre::Vector3 velVec = { 0,0,0 };

	TimeManager* tm = nullptr;

public:
	void Start();
	HouseGuy(GameObject* obj) : Enemy(obj) {};
	~HouseGuy();
	void LoadFromFile(json obj);
	virtual void Update();
	virtual void OnDeath();
	virtual void Spawn();
	void SpawnEnemy();



};
