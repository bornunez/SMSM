#pragma once
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "../../../Src/Game/Componentes/Player/PlayerCollision.h"
#include "../../../Src/MotorEngine/GUIManager.h"

#include <vector>

enum collisionID {PlayerID, BulletID, EnemyID, HeartID, ShotgunID, ShotgunBulletID};

enum WeaponEnum {Pistol, ShotGun};
enum HabilityEnum { None, SlowTime, FreezeTime };
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

	// Weapons
	WeaponEnum currentWeapon = WeaponEnum::Pistol;
	GameObject* pistolsGO;
	GameObject* shotgunGO;
	bool shotgunUnlocked = false;

	// Habilities
	void habilitiesLogic();
	float gameSpeed = 1;

	HabilityEnum currentHability = HabilityEnum::None;
	float timeElapsed = 0;

	float slowTimeSpeed = 0.3f;
	float slowTimeDuration = 3;
	float slowTimeCooldown = 5;
	float slowTimeCooldownTimer = 0;

	float freezeTimeSpeed = 0.01f;
	float freezeTimeDuration = 5;
	int freezeTimeEnemyCount = 0;
	int freezeTimeEnemiesNeeded = 10;

	// Control de invulnerabilidad
	bool invulnerability = false;
	float recoverTime;
	float actRecoverTime = 0;

	// HUD
	std::vector<CEGUI::Window *> livesHeart;
	CEGUI::Window* pistolWindow;
	CEGUI::Window* shotGunWindow;
	CEGUI::Window* slowTimeWindow;
	CEGUI::Window* slowTimeIndicator;
	CEGUI::Window* stopTimeWindow;
	CEGUI::Window* stopTimeIndicator;

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
	bool isTimeStopped() { return (gameSpeed == freezeTimeSpeed); }

	void increaseEnemyKillCount() { 
		freezeTimeEnemyCount++;
		if (freezeTimeEnemyCount <= freezeTimeEnemiesNeeded)
		{
#ifndef _DEBUG
			stopTimeIndicator->setSize(CEGUI::USize(CEGUI::UDim(((float)freezeTimeEnemyCount / (float)freezeTimeEnemiesNeeded) * 0.1, 0), CEGUI::UDim(0.03, 0)));
#endif
		}
	}

	void switchWeapon(WeaponEnum w);
	void unlockWeapon(WeaponEnum w);
	void activateShotgunHUD() { shotGunWindow->show(); };
};