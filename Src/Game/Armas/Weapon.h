#pragma once
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
class MeshRenderer;
class Weapon : public Component
{
private:
	string animationPassed = "null";

	bool canShoot = false;
	//Run
	float moveSpeed = 1;
	float runSpeed = 1;

	//Shoot
	float actTimePerShot = 0;
	float timePerShot = 1;
		//Animation
		float shootSpeed = 1;

	//Reload
	float actReloadTime = 0;
	float reloadTime = 2;
		//Animation
		float reloadSpeed = 1;

	//Magazine
	int actMagazine = 0;
	int magazine = 6;

	//For shotgun or similar
	int bulletsAmount = 1;
	float dispersion = 0;

	MeshRenderer* meshRend;
	void reloads();
public:
	Weapon(GameObject* obj) : Component(obj) {};
	~Weapon();
	void LoadFromFile(json obj);
	virtual void Update();
	void handleInput();
	void Start();
	void shoot();
	void reload();
};

