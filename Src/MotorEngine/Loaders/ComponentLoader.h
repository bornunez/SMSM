//#include "Game.h"
#include "..\Component.h"
#include "..\ResourcesManager//JsonParser.h"
#include <list>

class ComponentLoader
{
public:
	ComponentLoader();
	~ComponentLoader();

	virtual std::list<Component*> LoadComponents(json json, GameObject* gameObject) {};
	virtual Component* ParseComponent(json json, GameObject* gameObject) {};

};
