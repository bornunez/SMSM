#include "Weapon.h"
#include "../../Src/MotorEngine/MeshRenderer.h"
#include "../../../Src/MotorEngine/InputManager.h"


Weapon::~Weapon()
{
}
void Weapon::Start()
{
	meshRend->InitAnimations();
}
void Weapon::LoadFromFile(json obj)
{
	std::list<Component*> comps = gameObject->getComponents();
	bool found = false;
	auto it = comps.begin();
	while (!found && it != comps.end())
	{
		MeshRenderer* c = dynamic_cast<MeshRenderer*>(*it);
		if (c != nullptr) {
			found = true;
			meshRend = c;
		}
	}
	magazine = obj["magazine"];
	timePerShot = obj["timePerShoot"];
	reloadTime = obj["reloadTime"];
	bulletsAmount = obj["numBullets"];
	dispersion = obj["dispersion"];
}

void Weapon::Update()
{
	reloads();
	handleInput();
}

void Weapon::handleInput()
{
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Left))
	{
		shoot();
	}
}
void Weapon::reloads()
{
	if (actReloadTime > reloadTime)
	{
		if (actTimePerShot > timePerShot)
		{
			canShoot = true;
			if (meshRend->AnimationHasEnded("Shoot") || meshRend->AnimationHasEnded("Reload"))
			{
				meshRend->PlayAnimation("Move", true, false);
				meshRend->AnimationSpeed(1.75);
			}
		}
		else actTimePerShot += TimeManager::getInstance()->getDeltaTime();
	}
	else
	{
		actReloadTime += TimeManager::getInstance()->getDeltaTime();
		actTimePerShot += TimeManager::getInstance()->getDeltaTime();
	}
}

void Weapon::shoot()
{
	if (canShoot)
	{
		canShoot = false;
		if (actMagazine < magazine)
		{
			meshRend->PlayAnimation("Shoot", false);
			meshRend->AnimationSpeed(2.75);
			std::cout << "DISPARO";
			actMagazine += 1;
			actTimePerShot = 0;
		}
		else
		{
			reload();
		}
	}
}

void Weapon::reload()
{
	meshRend->PlayAnimation("Reload", false);
	meshRend->AnimationSpeed(2.5);
	actMagazine = 0;
	actReloadTime = 0;
}
