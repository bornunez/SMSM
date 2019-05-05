#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/MotorEngine/InputManager.h"
#include "RoomManager.h"

class MapHandler : public Component
{
public:
	MapHandler(GameObject* o) : Component(o) { };
	virtual ~MapHandler();
	
	void OnDestroy();
};

