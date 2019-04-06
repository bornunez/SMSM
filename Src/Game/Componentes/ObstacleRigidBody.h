#pragma once
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class ObstacleRigidBody:
	public RigidBodyComponent
{
public:
	ObstacleRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual ~ObstacleRigidBody();
};

