#pragma once
#include <vector>
#include "../../../Src/MotorEngine/Scene.h"
#include "../../Mapa/Room.h"
#include "../../Mapa/RoomManager.h"

class EnemyRigidBody;
class Enemy : public Component
{
private:

protected:
	int HP = 0;
	float moveSpeed = 0;
	bool alive = true;
	GameObject* player = nullptr;
	EnemyRigidBody* rbComp = nullptr;
	btRigidBody* rb = nullptr;
	Room* currRoom = nullptr;
public:
	void Start();

	Enemy(GameObject* obj) : Component(obj) {};
	~Enemy();
	virtual void LoadFromFile(json obj) = 0;
	virtual void Update() = 0;
	virtual void OnHit();
	virtual void OnDeath();
	virtual void Spawn() = 0;

};

