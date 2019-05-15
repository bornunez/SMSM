#pragma once
#include "../../Src/MotorEngine/TimeManager.h"
#include "../../../Src/MotorEngine/Component.h"
#include "WeaponBullet.h"

class MeshRenderer;
class Weapon : public Component
{
private:
	string animationPassed = "null";

	bool canShoot = false;
	int dualInt = 0;
	//Run
	float moveSpeed = 1;
	float runSpeed = 1;

	//Shoot
	float actTimePerShot = 0;
	float timePerShot = 1;
	int damage = 0;
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
	void AudioShoot();
	string shootAudio;
	void PhysicShoot();
	void reloads();

	float specialReloadTime = 0.65;
	float actSpecialReload = 0.65;
	bool specialReloading = false;
	Vector3 offset;
public:
	Weapon(GameObject* obj) : Component(obj) {};
	~Weapon();
	void LoadFromFile(json obj);
	virtual void Update();
	void handleInput();
	void Start();
	void directionalShoot(float randOff, bool shotgun);
	void shoot();
	void reload();
	void SpecialReload();
	Vector3 getOffset() { return offset; };
	float getDispersion() { return dispersion; };
	int getDamage() { return damage; };
};