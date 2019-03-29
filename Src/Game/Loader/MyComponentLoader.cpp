#include "MyComponentLoader.h"


MyComponentLoader::MyComponentLoader()
{
}


MyComponentLoader::~MyComponentLoader()
{
}

std::list<Component*> MyComponentLoader::LoadComponents(json json, GameObject * gameObject)
{
	std::list<Component*> components;
	for (auto &comp : json)
		if (comp.is_object()) {
			cout << "Loading component [ " << comp["type"] << " ]" << endl;
				Component* c = ParseComponent(comp, gameObject);
				if(c != nullptr)
					components.push_back(c);
		}
	return components;
}

Component * MyComponentLoader::ParseComponent(json comp , GameObject * gameObject)
{
	Component* c = nullptr;
	string c_type = comp["type"];

	//Crear instancia del componente en funcion del tipo
	if (c_type == "meshRenderer")
		c = new MeshRenderer(gameObject);
	if (c_type == "Weapon")
		c = new Weapon(gameObject);

	if (c != nullptr) {
		gameObject->AddComponent(c);
		c->LoadFromFile(comp);
	}

	return c;

}
