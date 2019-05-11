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
	virtual void Update();

private:
	CEGUI::UDim scrollSpeed = CEGUI::UDim(0.01,0.01);
	float timeToGoToMenu = 10;
	float currentTime = 0;

	std::vector<CEGUI::UVector2> originalPos;

	void resetPositions();
};

