#pragma once
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/Game/Componentes/Player/PlayerCollision.h"
#include "../../../Src/MotorEngine/GUIManager.h"

#include <vector>


class MyCamera;
class PlayerController: public Component
{
private:
	// Components
	PlayerCollision * playerColl;
	btRigidBody* playerRb;
	GameObject* brazo;

	// Camera
	MyCamera* cam;

	InputManager* input = nullptr;

	// Mouse
	float mouseSensitivity;
	int lastMouseX;
	float yAngle = 0;

	// Attributes
	float speed = 0;
	int maxHealth = 3;
	int lives = 3;
	int sensitivityLevel = 2;

	std::vector<CEGUI::Window *> livesHeart;

	void updateLivesHeart(); // Se debe llamar cuando el player reciba da�o

public:
	PlayerController(GameObject* obj) : Component(obj) {};
	~PlayerController();
	void LoadFromFile(json obj);
	void Start();
	virtual void Update();
	void handleInput();

	void modifySensitivity(bool v);
	
	void receiveDamage();
	void gainHealth();

	Vector3 getPlayerDirection();
};