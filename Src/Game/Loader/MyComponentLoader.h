#pragma once
#include "../../MotorEngine/Loaders/ComponentLoader.h"

#include "../Armas/Weapon.h"
#include "../Componentes/ObstacleRigidBody.h"
#include "../../MotorEngine/MyCamera.h"
#include "../Componentes/Player/PlayerCollision.h"
#include "../Componentes/Player/PlayerMov.h"
#include "../Componentes/Enemies/EnemyTest.h"
#include "../Componentes/Enemies/EnemyTest.h"
#include "../Componentes/Enemies/EnemyRigidBody.h"
#include "../Componentes/Enemies/KnifeGuy.h"
#include "../Componentes/Enemies/ShyGuy.h"
#include "../Componentes/GUILoader.h"

class MyComponentLoader : public ComponentLoader
{
public:
	MyComponentLoader();
	virtual ~MyComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject);
	virtual Component* ParseComponent(json comp, GameObject* gameObject);
};

