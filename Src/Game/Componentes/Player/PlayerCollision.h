#pragma once
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class PlayerCollision : public RigidBodyComponent
{
public:
	PlayerCollision(GameObject* obj);
	~PlayerCollision();
	//No hace nada más adicional que el metodo de su padre
	/*void LoadFromFile(json obj);*/
	virtual void collisionHandler(int id);
	virtual void Update();
	virtual void Start();
};

