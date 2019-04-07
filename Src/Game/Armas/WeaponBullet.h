#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class WeaponBullet:public RigidBodyComponent
{
public:
	WeaponBullet(GameObject* obj);
	~WeaponBullet();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

