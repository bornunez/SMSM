#pragma once
#include "RigidBodyComponent.h"
class EnemyRigidBody :
	public RigidBodyComponent
{
public:
	EnemyRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	
	virtual void collisionHandler(int id);
	virtual ~EnemyRigidBody();
};

