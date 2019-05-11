#pragma once
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class ItemRigidBody:
	public RigidBodyComponent
{
public:
	ItemRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	void LoadFromFile(json obj);
	virtual void collisionHandler(int id);
	virtual ~ItemRigidBody();
};

