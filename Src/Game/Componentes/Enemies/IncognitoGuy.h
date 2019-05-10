#pragma once
#include "Enemy.h"
#include <vector>
#include <algorithm>
#include <random>
#include "../../Src/MotorEngine/TimeManager.h"

class MeshRenderer;
class IncognitoGuy :
	public Enemy
{
private:
	enum state{IDLE, DEAD};
	state estado = state::IDLE;
	MeshRenderer* meshRend = nullptr;
	float scale = 0;
	float tpTime = 0;
	float tpTimer = 0;
	float tpFactor = 0;
	default_random_engine rng;

	std::vector<int> xVec;
	std::vector<int> zVec;
	int posIndex = 0;

	TimeManager* tm = nullptr;

public:
	void Start();
	IncognitoGuy(GameObject* obj) : Enemy(obj) {};
	~IncognitoGuy();
	void LoadFromFile(json obj);
	virtual void Update();
	virtual void OnDeath();
	virtual void Spawn();
	void Teleport();
	void RandomizeVecs();



};

