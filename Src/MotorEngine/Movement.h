#pragma once
#include "Component.h"

class Movement :
	public Component
{
public:
	Movement(GameObject* o);
	virtual ~Movement();

	void Update();
};

