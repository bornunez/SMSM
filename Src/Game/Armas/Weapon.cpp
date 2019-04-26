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
	offset = Vector3(obj["shootPosX"], obj["shootPosY"], obj["shootPosZ"]);
}

void Weapon::Update()
{
	reloads();
	handleInput();
	SpecialReload();
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
	if (bulletsAmount > 1)
	{
		for (int i = 0; i < bulletsAmount; i++)
		{
			int disp = dispersion;
			float randX = rand() % (disp)-(disp / 2);
			float randY = rand() % (disp)-(disp / 2);
			scene->Instantiate("Bullet",(gameObject->getPosition() + Vector3(randX*0.01, 0.05 + randY * 0.01, -0.5)), 0.01);
		}
	}
	//else 
		//scene->Instantiate("Bullet", (gameObject->getPosition() + Vector3(0, 0.05, -0.5)), 0.01);
	
	//cout << "Disparo en: [ " <<gameObject->getGlobalPosition().x << ", "<< gameObject->getGlobalPosition().y <<", " << gameObject->getGlobalPosition().z << " ]" << endl;
	
	//scene->Instantiate("Bullet", (gameObject->getGlobalPosition()+Vector3(0, -0.2, -0.5)), 0.05);

	Vector3 dir = gameObject->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	dir *= offset.z;
	//scene->Instantiate("Bullet", gameObject->getChild("ShootPoint")->getGlobalPosition(), 0.01);
	// gameObject->getGlobalPosition() + forward*0.4 + Vector3(0, -0.22, 0), 0.01);
	dir = Quaternion(Degree(offset.x), Vector3::UNIT_Y) * dir;
	//scene->Instantiate("Bullet", gameObject->getParent()->getNode()->getPosition() + dir + Vector3(0, offset.y, 0), 0.1);
	scene->Instantiate("Bullet", gameObject->getParent()->getGlobalPosition()+dir+Vector3(0, offset.y, 0), 0.01);
	//cout << "Disparo en: [ " << gameObject->getParent()->getNode()->getPosition() +dir << " ]" << endl;
	cout << "Direccion: " << gameObject->getParent()->getNode()->getPosition() + dir + Vector3(0, offset.y, 0);
	//cout << "Direccion: " << gameObject->getParent()->getNode()->getPosition();
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
	animationPassed = "Reload";
	meshRend->PlayAnimation("Reload", false);
	meshRend->AnimationSpeed(reloadSpeed);
	actMagazine = 0;
	actReloadTime = 0;
	if (gameObject->getName() == "Escopeta")
	{
		actSpecialReload = 0;
		specialReloading = true;
	}
}
void Weapon::SpecialReload()
{
	if (specialReloading && actSpecialReload < specialReloadTime) actSpecialReload += TimeManager::getInstance()->getDeltaTime();
	else if(specialReloading)
	{
		PrefabManager::getInstance()->Instantiate("ShotgunReloadBullet", scene, nullptr, (gameObject->getPosition() + Vector3(-0.1, 0.05, -0.5)), 0.01);
		specialReloading = false;
	}
}
