#pragma once
#include <vector>
#include "../../../Src/MotorEngine/Scene.h"

class EnemyRigidBody;
class Enemy : public Component
{
private:

protected:
	int HP = 0;
	int moveSpeed = 0;
	GameObject* player = nullptr;
	EnemyRigidBody* rbComp = nullptr;
	btRigidBody* rb = nullptr;

public:
	void Start();

	Enemy(GameObject* obj) : Component(obj) {};
	~Enemy();
	virtual void LoadFromFile(json obj) = 0;
	virtual void Update() = 0;
	virtual void OnHit() = 0;
	virtual void OnDeath() = 0;
	virtual void Spawn() = 0;

};

