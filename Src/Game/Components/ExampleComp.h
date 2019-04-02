#pragma once
#include "../../../Src/MotorEngine/Component.h"

class ExampleComp : public Component
{
public:
	ExampleComp(GameObject* _gameObject) : Component(_gameObject) {};
	void LoadFromFile(json obj);

	void Awake();

	~ExampleComp();
};

