#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/MotorEngine/InputManager.h"
#include "RoomManager.h"

class SkipRoom : public Component
{
public:
	SkipRoom(GameObject* o) : Component(o) { };
	virtual ~SkipRoom();
	void LoadFromFile(json obj);

	void Update();
};

