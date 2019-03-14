#pragma once
#include "Component.h"

class Movement :
	public Component
{
public:
	Movement(GameObject* o);
	virtual ~Movement();

	virtual void receiveMessage(string message);

	void Update();
};

