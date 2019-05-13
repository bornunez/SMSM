#pragma once
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/Game/Componentes/Player/PlayerCollision.h"
#include "../../../Src/MotorEngine/GUIManager.h"

#include <vector>

enum collisionID {PlayerID, BulletID, EnemyID, HeartID, ShotgunID};

enum WeaponEnum {Pistol, ShotGun};
class MeshRenderer;
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
	float walkSpeed = 0;
	float runSpeed = 0;
	int maxHealth = 3;
	int lives = 3;
	int sensitivityLevel = 2;
	float gameSpeed = 1;

	// Weapons
	WeaponEnum currentWeapon = WeaponEnum::Pistol;
	CEGUI::Window* pistolWindow;
	CEGUI::Window* shotGunWindow;
	GameObject* pistolsGO;
	GameObject* shotgunGO;
	bool shotgunUnlocked = false;

	// Control de invulnerabilidad
	bool invulnerability = false;
	float recoverTime;
	float actRecoverTime = 0;
	std::vector<CEGUI::Window *> livesHeart;

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
	void hideHud();
	void SetInvulnerability();

	Vector3 getPlayerDirection();

	float getGameSpeed() { return gameSpeed; }

	void switchWeapon(WeaponEnum w);
	void unlockWeapon(WeaponEnum w);
	void activateShotgunHUD() { shotGunWindow->show(); };
};