#pragma once
#include "../../MotorEngine/Loaders/ComponentLoader.h"

#include "../Armas/Weapon.h"
#include "../Componentes/ObstacleRigidBody.h"
#include "../../MotorEngine/MyCamera.h"
#include "../Componentes/Player/PlayerCollision.h"
#include "../Componentes/Player/PlayerController.h"
#include "../Componentes/Enemies/EnemyTest.h"
#include "../Componentes/Enemies/EnemyTest.h"
#include "../Componentes/Enemies/EnemyRigidBody.h"
#include "../Componentes/Enemies/KnifeGuy.h"
#include "../Componentes/Enemies/ShyGuy.h"
#include "../Componentes/Enemies/ButterGuy.h"
#include "../Componentes/Enemies//EnemyBullet.h"
#include "../Componentes/Enemies/HouseGuy.h"
#include "../Componentes/Enemies/BombGuy.h"
#include "../Componentes/Enemies/IncognitoGuy.h"
#include "../Componentes/Enemies/BossGuy.h"
#include "../Componentes/Enemies//SpawnOnDestroy.h"
#include "../Componentes/GUILoader.h"
#include "../Componentes/creditsManager.h"

class MyComponentLoader : public ComponentLoader
{
public:
	MyComponentLoader();
	virtual ~MyComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject);
	virtual Component* ParseComponent(json comp, GameObject* gameObject);
};

