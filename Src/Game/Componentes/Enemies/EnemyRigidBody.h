#pragma once
#include "../../../Src/MotorEngine/RigidBodyComponent.h"
#include "Enemy.h"


class EnemyRigidBody :
	public RigidBodyComponent
{
private:
	Enemy* enemy = nullptr;
	int gunDamage = 0;
	int shotgunDamage = 0;

public:
	virtual void Start();
	EnemyRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
	virtual ~EnemyRigidBody();
};

