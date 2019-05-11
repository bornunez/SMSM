#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"
#include "../Player/PlayerController.h"

class EnemyBullet: public RigidBodyComponent
{
private:
	float speed = 1;
	int grav;
	float linDamp, angDamp;
	float deathTime = 0;
	float deathTimer = 0;
	Vector3 direccion;
	int ownerID;
	PlayerController* playerController;

public:
	EnemyBullet(GameObject* obj);
	~EnemyBullet();
	void Start();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

