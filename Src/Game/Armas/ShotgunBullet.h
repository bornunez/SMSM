#pragma once
#include "../../Src/MotorEngine/TimeSwitch.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"
#include "../Componentes/Player/PlayerController.h"

float const INVERSION_ROT_MAX =  1.34f;
float const INVERSION_ROT_MIN = 0.74f;

class ShotgunBullet : public RigidBodyComponent
{
private:
	float speed = 1;
	int grav;
	float linDamp, angDamp;
	Vector3 direccion;
	Vector3 nuevaDir;
	PlayerController* playerController;
	float deathTime = 3;
	float actDeathTime = 0;

public:
	ShotgunBullet(GameObject* obj);
	~ShotgunBullet();
	void Start();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};