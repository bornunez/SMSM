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
	moveSpeed = obj["moveSpeed"];
	runSpeed = obj["runSpeed"];

	timePerShot = obj["timePerShoot"];
	shootSpeed = obj["shootSpeed"];

	reloadTime = obj["reloadTime"];
	reloadSpeed = obj["reloadSpeed"];

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
	//Handlea el input
	if (InputManager::getInstance()->getMouseButton(OIS::MouseButtonID::MB_Left))
	{
		shoot();
	}
	if ((meshRend->AnimationHasEnded("Shoot") || meshRend->AnimationHasEnded("Reload") || animationPassed == "Move"))
	{
		if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_W) || 
			(InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) && (animationPassed == "Shoot" || animationPassed == "Reload")))
		{
			animationPassed = "Move";
			meshRend->PlayAnimation("Move", true, false);
			meshRend->AnimationSpeed(moveSpeed);
		}
		else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_W))
		{
			animationPassed = "Move";
			meshRend->PlayAnimation("Move", true, true);
			meshRend->AnimationSpeed(moveSpeed);
		}
	}
	if (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_W) && meshRend->isPlaying("Move"))
	{
		animationPassed = "Move";
		meshRend->PlayAnimation("Move", true, false);
		meshRend->StopAnimation(true);
		meshRend->AnimationSpeed(moveSpeed);
	}
}
void Weapon::reloads()
{
	if (actReloadTime > reloadTime)
	{
		if (actTimePerShot > timePerShot)
		{
			canShoot = true;
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
			animationPassed = "Shoot";
			meshRend->PlayAnimation("Shoot", false);
			meshRend->AnimationSpeed(shootSpeed);
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
	animationPassed = "Reload";
	meshRend->PlayAnimation("Reload", false);
	meshRend->AnimationSpeed(reloadSpeed);
	actMagazine = 0;
	actReloadTime = 0;
}
