#pragma once
#include "GUILoader.h"
#include "../../../Src/MotorEngine/GUIManager.h"
#include "../../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/InputManager.h"


class creditsManager :
	public GUILoader
{
public:
	creditsManager(GameObject* obj) : GUILoader(obj) {};
	~creditsManager();
	virtual void LoadFromFile(json obj);
};

