#pragma once
#include "c:\Users\Miniman\Desktop\Cosas de la uni\TERCERO\Segundo Cuatri\Proyecto 3\Proyecto 3\SMSM\SMSM\Src\MotorEngine\Component.h"
class Movement :
	public Component
{
public:
	Movement(GameObject* o);
	virtual ~Movement();

	void Update();
};

