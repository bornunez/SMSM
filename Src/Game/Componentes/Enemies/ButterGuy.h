#pragma once
#include "Enemy.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../Armas/WeaponBullet.h"

class MeshRenderer;
class ButterGuy :
	public Enemy
{
private:
	enum state{IDLE, FLEEING, AIMING, DEAD};
	state estado = state::IDLE;
	MeshRenderer* meshRend = nullptr;
	float dist = 0;
	float distFactor = 0;

	float scale = 0;
	float shootTime = 0;
	float shootTimer = 0;

	float defAnimSp = 0;
	float deathAnimSp = 0;

	TimeManager* tm = nullptr;

	void Start();
	void LoadFromFile(json obj);
	void Shoot();

protected:
	virtual void Update();
	virtual void OnDeath();

public:
	ButterGuy(GameObject* obj) : Enemy(obj) {};
	~ButterGuy();
};

