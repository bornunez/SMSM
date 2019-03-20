#include "MyComponentLoader.h"




MyComponentLoader::MyComponentLoader()
{
}


MyComponentLoader::~MyComponentLoader()
{
}

std::list<Component*> MyComponentLoader::LoadComponents(json json, GameObject * gameObject)
{
	for (auto &pref : json)
		if (pref.is_object()) {
			cout << "Loading component [ " << pref["componentType"] << " ]" << endl;
			
		}
	return std::list<Component*>();
}

Component * MyComponentLoader::ParseComponent(json json, GameObject * gameObject)
{
	return nullptr;
}
