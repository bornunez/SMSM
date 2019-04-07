#pragma once
#include "../../Src/MotorEngine/TimeManager.h"
#include <vector>
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/RigidBodyComponent.h"

class EnemyTest : public RigidBodyComponent
{
private:
	string animationPassed = "null";

	//Run
	float moveSpeed = 1;
	float moveTime = 1;
	float moveTimer = 0;
	int curDir = 0;
	std::vector<btVector3> dirs = std::vector<btVector3>{ btVector3(1,0,0), btVector3(0,0,1), btVector3(-1,0,0), btVector3(0,0,-1) };
	TimeManager* tm;

public:
	EnemyTest(GameObject* obj) : RigidBodyComponent(obj) {};
	~EnemyTest();
	void LoadFromFile(json obj);
	virtual void Update();
	virtual void collisionHandler(int id);
	void Start();
};

