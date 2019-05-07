#pragma once
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class PlayerCollision : public RigidBodyComponent
{
public:
	PlayerCollision(GameObject* obj);
	~PlayerCollision();
	virtual void collisionHandler(int id);
	virtual void Update();
	virtual void Start();

	void receiveDamage();
	void gainHealth();

private:

	bool canGetDamage = true;
	float inmunityTime = 1.0f;
};

