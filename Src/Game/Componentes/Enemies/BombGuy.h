#pragma once
#include "Enemy.h"
#include "../../Src/MotorEngine/TimeManager.h"

class MeshRenderer;
class BombGuy :
	public Enemy
{
private:
	enum state{IDLE, CHASING, EXPLODING, DEAD};
	state estado = state::IDLE;
	MeshRenderer* meshRend = nullptr;
	float chaseDist = 0;
	float expDist = 0;
	float expTime = 0;
	float expRadius = 0;
	float scale = 0;

	float defAnimSp = 0;
	float deathAnimSp = 0;

	float expTimer = 0;	
	TimeManager* tm = nullptr;

	void Start();
	void LoadFromFile(json obj);
	void Explode();

protected:
	virtual void Update();
	virtual void OnDeath();

public:
	BombGuy(GameObject* obj) : Enemy(obj) {};
	~BombGuy();
};

