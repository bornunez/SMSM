#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class WeaponBullet: public RigidBodyComponent
{
private:
	float speed = 1;
	int grav;
	float linDamp, angDamp;
	Vector3 direccion;

public:
	WeaponBullet(GameObject* obj);
	~WeaponBullet();
	void Start();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

