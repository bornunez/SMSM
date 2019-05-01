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

public:
	void Start();
	ShyGuy(GameObject* obj) : Enemy(obj) {};
	~ShyGuy();
	void LoadFromFile(json obj);
	void Update();
	void OnHit();
	void OnDeath();
	void Spawn();



};

