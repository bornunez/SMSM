#pragma once
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"

class PlayerMov : public Component
{
public:
	PlayerMov(GameObject* obj) : Component(obj) {};
	~PlayerMov();
	void LoadFromFile(json obj);
	virtual void Update();
	void handleInput();
	void Start();
};

