#pragma once
#include "Component.h"
#include "TimeManager.h"

class RigidBodyComponent :
	public Component
{
public:
	RigidBodyComponent(GameObject* _gameObject) : Component(_gameObject) {};
	virtual ~RigidBodyComponent();

	//En función del identificador que reciba el metodo que hereda de esta clase realizará un efecto u otro
	virtual void collisionHandler(int id) {};
	virtual void Update();
	void resetHitState();

protected:
	bool hit = false;
	float recoilTime = 0.1f;

private:
	float timeSinceLastHit = 0;
};