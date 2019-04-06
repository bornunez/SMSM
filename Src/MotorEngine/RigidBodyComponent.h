#pragma once
#include <btBulletDynamicsCommon.h>
#include "Component.h"
#include "TimeManager.h"

class RigidBodyComponent :
	public Component
{
public:
	RigidBodyComponent(GameObject* _gameObject) : Component(_gameObject) {};
	virtual void LoadFromFile(json obj);
	virtual ~RigidBodyComponent();

	//En función del identificador que reciba el metodo que hereda de esta clase realizará un efecto u otro
	virtual void collisionHandler(int id) {};
	virtual void Update();
	void resetHitState();

	bool getIsTrigger() { return isTrigger; };

protected:
	bool hit = false;
	bool isTrigger = true;
	float recoilTime = 0.1f;

	btRigidBody* physicRB;

private:
	float timeSinceLastHit = 0;
};