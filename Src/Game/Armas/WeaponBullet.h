#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/MotorEngine/Scene.h"
class WeaponBullet:public Component
{
private:
	RigidBodyComponent* rb;
public:
	WeaponBullet(GameObject* obj);
	~WeaponBullet();
	virtual void Update();
};

