#pragma once
#include "Enemy.h"

class MeshRenderer;
class KnifeGuy :
	public Enemy
{
private:
	enum state{ALIVE, DEAD};
	state estado = state::ALIVE;
	MeshRenderer* meshRend = nullptr;

public:
	void Start();
	KnifeGuy(GameObject* obj) : Enemy(obj) {};
	~KnifeGuy();
	void LoadFromFile(json obj);
	virtual void Update();
	virtual void OnHit();
	virtual void OnDeath();
	virtual void Spawn();



};

