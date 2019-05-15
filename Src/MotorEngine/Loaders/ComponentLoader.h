#pragma once

#include "..\ResourcesManager//JsonParser.h"
#include <list>
#include "../Component.h"
#include "../GameObject.h"
#include "../MeshRenderer.h"
#include "../Components/RandomMeshRenderer.h"
#include "../Components/MapLoader.h"

class ComponentLoader
{
public:
	ComponentLoader();
	~ComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject) { return std::list<Component*>(); };
	virtual Component* ParseComponent(json json, GameObject* gameObject) { return nullptr; };

};
