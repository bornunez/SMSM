#pragma once
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/Game/Componentes/Player/PlayerCollision.h"
#include "../../../Src/MotorEngine/Loaders/PrefabManager.h"

class PlayerMov : public Component
{
private:
	PlayerCollision * playerColl;
	btRigidBody* playerRb;
	float maxRotSpeed = 10;
	float movSpeed = 0.5;
	float maxSpeed = 20;
	float speed = 0;

public:
	PlayerMov(GameObject* obj) : Component(obj) {};
	~PlayerMov();

	Vector3 getPlayerDirection();

	void LoadFromFile(json obj);
	virtual void Update();
	void handleInput();
	void Start();
	void Awake();
};

