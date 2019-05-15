#pragma once
#include "Enemy.h"

class MeshRenderer;
class ShyGuy :
	public Enemy
{
private:
	enum state{IDLE, FLEEING, DEAD};
	state estado = state::IDLE;
	MeshRenderer* meshRend = nullptr;
	float dist = 0;
	float distFactor = 0;
	int minFactor = 0;
	int maxFactor = 0;
	float scale = 0;
	float defAnimSp = 0;
	float deathAnimSp = 0;

	void Start();
	void LoadFromFile(json obj);

protected: 
	virtual void Update();
	virtual void OnDeath();

public:
	ShyGuy(GameObject* obj) : Enemy(obj) {};
	~ShyGuy();
};

