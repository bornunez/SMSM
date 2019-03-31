#pragma once
#include "../../../Src/MotorEngine/RigidBodyComponent.h"


class EnemyRigidBody :
	public RigidBodyComponent
{
public:
	EnemyRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual ~EnemyRigidBody();
};

