#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"
#include "../Componentes/Player/PlayerController.h"

class WeaponBullet: public RigidBodyComponent
{
private:
	float speed = 1;
	int grav;
	float actGrav = 0;
	float linDamp, angDamp;
	Vector3 direccion;
	PlayerController* playerController;
	float deathTime = 3;
	float actDeathTime = 0;

public:
	WeaponBullet(GameObject* obj);
	~WeaponBullet();
	void Start();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

