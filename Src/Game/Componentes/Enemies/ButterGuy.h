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

	TimeManager* tm = nullptr;

public:
	void Start();
	ButterGuy(GameObject* obj) : Enemy(obj) {};
	~ButterGuy();
	void LoadFromFile(json obj);
	void Update();
	void OnDeath();
	void Spawn();
	void Shoot();



};

