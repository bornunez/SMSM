#pragma once
#include "../../../../Src/MotorEngine/Component.h"
class PlayerTrigger : public Component
{
protected:
	float offset_x, offset_y;
	float width, height;
	GameObject* player;

public:
	PlayerTrigger(GameObject* o);
	PlayerTrigger(GameObject* o, int w, int h, int _offset_x = 0, int _offset_y = 0);
	virtual void LoadFromFile(json obj);

	virtual void Awake();
	virtual void Update();

	virtual void OnTriggerEnter() {};

	~PlayerTrigger();
};