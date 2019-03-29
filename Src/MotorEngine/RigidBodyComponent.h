#pragma once
#include "Component.h"
class RigidBodyComponent :
	public Component
{
public:
	RigidBodyComponent(GameObject* _gameObject) : Component(_gameObject) {};
	virtual ~RigidBodyComponent();

	//En funci�n del identificador que reciba el metodo que hereda de esta clase realizar� un efecto u otro
	virtual void collisionHandler(int id) {};
};