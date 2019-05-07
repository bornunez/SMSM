#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/MotorEngine/InputManager.h"

class SpawnOnDestroy : public Component
{
protected:
	std::vector<string> objectsToSpawn;
public:
	SpawnOnDestroy(GameObject* o) : Component(o) { };
	virtual ~SpawnOnDestroy();
	void LoadFromFile(json obj);

	void OnDestroy();
};

