#pragma once
#include "../../MotorEngine/Loaders/ComponentLoader.h"

#include "../Armas/Weapon.h"
#include "../Componentes/EnemyRigidBody.h"


class MyComponentLoader : public ComponentLoader
{
public:
	MyComponentLoader();
	virtual ~MyComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject);
	virtual Component* ParseComponent(json comp, GameObject* gameObject);
};

