#pragma once
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class PlayerCollision : public RigidBodyComponent
{
public:
	PlayerCollision(GameObject* obj);
	~PlayerCollision();
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual void Update();
};

