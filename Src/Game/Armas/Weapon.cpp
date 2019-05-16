#include "Weapon.h"
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
	meshRend = gameObject->getComponent<MeshRenderer>();

	damage = obj["damage"];
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

	string shotAudio = obj["shootAudio"];
	shootAudio = shotAudio;
}

void Weapon::Update()
{
	reloads();
	handleInput();
	SpecialReload();
}

//Handlea todo el input y animaciones
void Weapon::handleInput()
{
	if (!InputManager::getInstance()->getKey(OIS::KC_LSHIFT) && 
	   ((InputManager::getInstance()->getMouseButton(OIS::MouseButtonID::MB_Left) && dualInt == 0)||
		(InputManager::getInstance()->getMouseButton(OIS::MouseButtonID::MB_Right) && dualInt == 1)))
	{
		shoot();
	}
	if (((animationPassed == "Move" || animationPassed == "Run" || animationPassed == "Shoot") && meshRend->AnimationHasEnded("Shoot")) && !InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) && !InputManager::getInstance()->getKey(OIS::KeyCode::KC_A) &&
		!InputManager::getInstance()->getKey(OIS::KeyCode::KC_D) && !InputManager::getInstance()->getKey(OIS::KeyCode::KC_S))
	{
		meshRend->PlayAnimation("Move", true, false);
		meshRend->StopAnimation(true);
	}
	if ((meshRend->AnimationHasEnded("Shoot") || meshRend->AnimationHasEnded("Reload") || animationPassed == "Move"))
	{
		if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_W) || 
			(InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) && (animationPassed == "Shoot" || animationPassed == "Reload"))||
			InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_A) ||
			(InputManager::getInstance()->getKey(OIS::KeyCode::KC_A) && (animationPassed == "Shoot" || animationPassed == "Reload")) || 
			InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_D) ||
			(InputManager::getInstance()->getKey(OIS::KeyCode::KC_D) && (animationPassed == "Shoot" || animationPassed == "Reload")) || 
			InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_S) ||
			(InputManager::getInstance()->getKey(OIS::KeyCode::KC_S) && (animationPassed == "Shoot" || animationPassed == "Reload")))
		{
			animationPassed = "Move";
			meshRend->PlayAnimation("Move", true, false);
			meshRend->SetAnimationSpeed(moveSpeed);
		}
		else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) ||
			InputManager::getInstance()->getKey(OIS::KeyCode::KC_A) || 
			InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) ||
			InputManager::getInstance()->getKey(OIS::KeyCode::KC_D))
		{
			if (!InputManager::getInstance()->getKey(OIS::KeyCode::KC_LSHIFT))
			{
				animationPassed = "Move";
				meshRend->PlayAnimation("Move", true, true);
				meshRend->SetAnimationSpeed(moveSpeed);
			}
			else
			{
				animationPassed = "Run";
				meshRend->PlayAnimation("Run", true, true);
				meshRend->SetAnimationSpeed(runSpeed);
			}

		}
	}
	if ((!InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) && meshRend->isPlaying("Move")) 
		|| (!InputManager::getInstance()->getKey(OIS::KeyCode::KC_LSHIFT) && meshRend->isPlaying("Run")))
	{
		animationPassed = "Move";
		meshRend->PlayAnimation("Move", true, false);
		meshRend->StopAnimation(true);
		meshRend->SetAnimationSpeed(moveSpeed);
	}
}

//Controla los tiempos de recarga
void Weapon::reloads()
{
	if (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_R) && (!meshRend->isPlaying("Reload")||(meshRend->isPlaying("Reload") && meshRend->AnimationHasEnded("Reload"))))
	{
		reload();
	}
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

//Comportamiento de disparo desde c�digo
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
			meshRend->SetAnimationSpeed(shootSpeed);
#ifdef C_DEBUG
			std::cout << "DISPARO"<<endl;
#endif
			actMagazine += 1;
			actTimePerShot = 0;
		}
		else
		{
			reload();
		}
	}
}
//Gesti�n de disparo
void Weapon::PhysicShoot()
{
	if (bulletsAmount > 1)
	{
		for (int i = 0; i < bulletsAmount; i++)
		{
			directionalShoot(dispersion, true);
		}
	}
	else
	{
		directionalShoot(0, false);
	}
}

//Comportamiento f�sico de disparo
void Weapon::directionalShoot(float randOff, bool shotgun)
{
	Vector3 auxVec = scene->getGame()->getViewport()->getCamera()->getRealOrientation() * Vector3::NEGATIVE_UNIT_Z*offset.z;
	Vector3 auxVecFinal = Quaternion(Degree(offset.x), Vector3::UNIT_Y) * auxVec;
	Vector3 auxOff = auxVecFinal.perpendicular();
	auxOff = Quaternion(Degree(rand() % 360), auxVecFinal) * auxOff;

	if (shotgun)
	{
		float randDist = (float)(rand() % ((int)(randOff * 1000000))) / 1000000;
		auxVecFinal = auxVecFinal + (auxOff* randDist);
	}

	if(!shotgun)
		scene->Instantiate("Bullet", gameObject->getParent()->getGlobalPosition() + auxVecFinal + Vector3(0, offset.y, 0), 0.01);
	else scene->Instantiate("ShotgunBullet", gameObject->getParent()->getGlobalPosition() + auxVecFinal + Vector3(0, offset.y, 0), 0.01);
}

void Weapon::AudioShoot()
{
	if(dualInt == 0)
		AudioManager::getInstance()->playSound(shootAudio, false, 0.4, CHANNEL::Disparos);
	else
		AudioManager::getInstance()->playSound(shootAudio, false, 0.4, CHANNEL::Disparos2);
}

void Weapon::reload()
{
	animationPassed = "Reload";
	meshRend->PlayAnimation("Reload", false);
	meshRend->SetAnimationSpeed(reloadSpeed);
	actMagazine = 0;
	actReloadTime = 0;
	actTimePerShot = 0;
	canShoot = false;
	if (gameObject->getName() == "Escopeta")
	{
		actSpecialReload = 0;
		specialReloading = true;
	}
}

void Weapon::SpecialReload()
{
	if (specialReloading && actSpecialReload < specialReloadTime) 
		actSpecialReload += TimeManager::getInstance()->getDeltaTime();
	else if(specialReloading)
	{
		Vector3 dir = gameObject->getParent()->getParent()->getNode()->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
		dir *= offset.z*2;
		dir = Quaternion(Degree(-offset.x*3), Vector3::UNIT_Y) * dir;

		scene->Instantiate("ShotgunReloadBullet", gameObject->getParent()->getGlobalPosition() + dir + Vector3(0, 0.18, 0), 0.03);

#ifdef C_DEBUG
		cout << "Direccion: " << gameObject->getParent()->getNode()->getPosition() + dir + Vector3(0, offset.y, 0)<<endl;
#endif
		specialReloading = false;
	}
}