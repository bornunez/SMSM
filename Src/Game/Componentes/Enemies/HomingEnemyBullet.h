#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"
#include "../Player/PlayerController.h"

class HomingEnemyBullet: public RigidBodyComponent
{
private:
	float speed = 0;
	int grav = 0;
	float linDamp, angDamp;
	float deathTime = 0;
	float deathTimer = 0;
	Vector3 direccion;
	int ownerID = 0;
	float scale = 0;
	PlayerController* playerController;

public:
	HomingEnemyBullet(GameObject* obj);
	~HomingEnemyBullet();
	void Start();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

