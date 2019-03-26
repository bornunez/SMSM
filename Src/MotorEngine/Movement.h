#pragma once
#include "Component.h"

class Movement :
	public Component
{
public:
	Movement(GameObject* o);
	virtual void LoadFromFile(json obj);
	virtual ~Movement();

	virtual void receiveMessage(string message);

	void Update();
};

