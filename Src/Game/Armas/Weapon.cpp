#include "Weapon.h"
//#include "../../Src/MotorEngine/MeshRenderer.h"
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/AudioManager.h"
#include "../../../Src/MotorEngine/Loaders/PrefabManager.h"

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
		it++;
	}
	//MeshRenderer* c = dynamic_cast<MeshRenderer*>(gameObject->getComponent<MeshRenderer>());
	magazine = obj["magazine"];
	moveSpeed = obj["moveSpeed"];
	runSpeed = obj["runSpeed"];
	dualInt = obj["dualInt"];

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
	if ((InputManager::getInstance()->getMouseButton(OIS::MouseButtonID::MB_Left) && dualInt == 0)||
		(InputManager::getInstance()->getMouseButton(OIS::MouseButtonID::MB_Right) && dualInt == 1))
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
			if (!InputManager::getInstance()->getKey(OIS::KeyCode::KC_LSHIFT))
			{
				animationPassed = "Move";
				meshRend->PlayAnimation("Move", true, true);
				meshRend->AnimationSpeed(moveSpeed);
			}
			else
			{
				animationPassed = "Run";
				meshRend->PlayAnimation("Run", true, true);
				meshRend->AnimationSpeed(runSpeed);
			}

		}
	}
	if ((InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_W) && meshRend->isPlaying("Move")) 
		|| (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_LSHIFT) && meshRend->isPlaying("Run")))
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
			AudioShoot();
			PhysicShoot();
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
void Weapon::PhysicShoot()
{
	cout << "Disparo en: [ " <<gameObject->getGlobalPosition().x << ", "<< gameObject->getGlobalPosition().y <<", " << gameObject->getGlobalPosition().z << " ]" << endl;
	scene->Instantiate("Bullet", (gameObject->getGlobalPosition()+Vector3(0, -0.2, -0.5)), 0.05);
}
void Weapon::AudioShoot()
{
	if(dualInt == 0)
		AudioManager::getInstance()->playSound("GunShoot", false, 0.4, CHANNEL::Disparos);
	else
		AudioManager::getInstance()->playSound("GunShoot", false, 0.4, CHANNEL::Disparos2);
}
void Weapon::reload()
{
	plane->SetActive(true);
	animationPassed = "Reload";
	meshRend->PlayAnimation("Reload", false);
	meshRend->AnimationSpeed(reloadSpeed);
	actMagazine = 0;
	actReloadTime = 0;
}
