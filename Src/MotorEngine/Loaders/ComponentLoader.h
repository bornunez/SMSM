#pragma once
//#include "Game.h"

#include "..\ResourcesManager//JsonParser.h"
#include <list>
#include "../Component.h"
#include "../GameObject.h"
#include "../MeshRenderer.h"

class ComponentLoader
{
public:
	ComponentLoader();
	~ComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject) { return std::list<Component*>(); };
	virtual Component* ParseComponent(json json, GameObject* gameObject) { return nullptr; };

};
