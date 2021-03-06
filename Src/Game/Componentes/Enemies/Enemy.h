#pragma once
#include <vector>
#include "../../../Src/MotorEngine/Scene.h"
#include "../../Mapa/Room.h"
#include "../../Mapa/RoomManager.h"
#include "../Player/PlayerController.h"

class EnemyRigidBody;
class Enemy : public Component
{
private:

protected:
	int HP = 0;
	float moveSpeed = 0;
	float gravity = 0;
	bool alive = true;
	int heartProb = 0;
	float hurtTime = 0.05;
	float currHurtTime = -60;

	GameObject* player = nullptr;
	PlayerController* playerController = nullptr;
	EnemyRigidBody* rbComp = nullptr;
	btRigidBody* rb = nullptr;
	Room* currRoom = nullptr;


public:
	void Start();

	Enemy(GameObject* obj) : Component(obj) {};
	~Enemy();
	virtual void LoadFromFile(json obj) = 0;
	virtual void Update() = 0;
	virtual void OnHit(int damage);
	virtual void OnDeath();
	void playSound(string name, bool loop, float volume);
	btQuaternion VecToQuat(Vector3 vec);
	btQuaternion VecToQuat(btVector3 vec);

};

