#pragma once


class Movement :
	public Component
{
public:
	Movement(GameObject* o);
	virtual ~Movement();

	void Update();
};

