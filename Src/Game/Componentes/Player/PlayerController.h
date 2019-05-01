#pragma once
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/Game/Componentes/Player/PlayerCollision.h"
#include "../../../Src/MotorEngine/GUIManager.h"


class MyCamera;
class PlayerController: public Component
{
private:
	// Components
	PlayerCollision * playerColl;
	btRigidBody* playerRb;

	// Camera
	MyCamera* cam;

	InputManager* input = nullptr;

	// Mouse
	float mouseSensitivity;
	int lastMouseX;

	// Attributes
	float speed = 0;
	int lives = 3;

	CEGUI::Window * livesHeart;

	void updateLivesHeart(); // Se debe llamar cuando el player reciba daño

public:
	PlayerController(GameObject* obj) : Component(obj) {};
	~PlayerController();
	void LoadFromFile(json obj);
	void Start();
	virtual void Update();
	void handleInput();

	Vector3 getPlayerDirection();
};