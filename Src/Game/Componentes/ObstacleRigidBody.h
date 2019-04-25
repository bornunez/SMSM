#pragma once
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class ObstacleRigidBody:
	public RigidBodyComponent
{
public:
	ObstacleRigidBody(GameObject *gameObject_) : RigidBodyComponent(gameObject_) {};
	void LoadFromFile(json obj);

	//No hace nada adicionalmente asi que no es necesario
//	virtual void Start();

	virtual void collisionHandler(int id);
	virtual ~ObstacleRigidBody();
};

