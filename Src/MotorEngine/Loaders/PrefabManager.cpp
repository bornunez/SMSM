#include "PrefabManager.h"
#include <filesystem>
#include <iostream>

PrefabManager* PrefabManager::instance = nullptr;

PrefabManager::PrefabManager()
{
}

PrefabManager::~PrefabManager()
{
}

PrefabManager * PrefabManager::getInstance()
{
	if (instance == nullptr)
		instance = new PrefabManager();
	return instance;
}

void PrefabManager::Init(string path, ComponentLoader* compLoader)
{
	SetRootFolder(path);
	componentLoader = compLoader;
}

void PrefabManager::SetRootFolder(string path)
{
	rootFolder = path;
}

struct path_leaf_string
{
	std::string operator()(const std::experimental::filesystem::directory_entry& entry) const
	{
		return entry.path().filename().string();
	}
};

void read_directory(const std::string& name, std::vector<string>& v)
{
	std::experimental::filesystem::path p(name);
	std::experimental::filesystem::directory_iterator start(p);
	std::experimental::filesystem::directory_iterator end;
	std::transform(start, end, std::back_inserter(v), path_leaf_string());
}
void PrefabManager::LoadAllPrefabs() 
{
	cout << endl << endl;
	cout << "=================================================" << endl;
	cout << "========       CARGA DE PREFABS        ==========" << endl;
	cout << "=================================================" << endl << endl;
	cout << "Cagando prefabs de la carpeta: " << rootFolder << endl << endl;
	std::vector<string> names;
	read_directory(rootFolder, names);
	for (string n : names) {
		LoadPrefab(rootFolder + n);
	}
	cout << "=================================================" << endl << endl;
}

void PrefabManager::LoadPrefab(string path)
{
	cout << "Cargando archivo: " << path << " ..." << endl ;
	json aux = JsonParser::ParseJsonFile(path);
	prefabs.insert(pair<string,json>(aux["prefabName"], aux));
	cout << "Cargado prefab con nombre: " << aux["prefabName"] << endl << endl ;
}

GameObject * PrefabManager::GenerateGameObject(string prefabName, Scene * scene, GameObject * parent)
{
	//Primero intentamos encontrar el json del prefab. Si no existe devolvemos algun tipo de error
	std::map<string,json>::iterator it = prefabs.find(prefabName);
	GameObject* o = nullptr;
	if (it != prefabs.end()) {
		json jsonObj = (*it).second;
		string objName = jsonObj["name"];
		cout << "Loaded prefab [ " << prefabName << " ]" << " and named it [ " << objName << " ]" << endl;
		o = new GameObject(scene, jsonObj["name"], jsonObj["active"],parent);

		componentLoader->LoadComponents(jsonObj["components"], o);
		//std::list<Component*> components = componentLoader->LoadComponents(jsonObj["components"], o);
		
	}
	else {
		cout << "ERROR: Prefab [ " << prefabName << " ]" << " doesn't exists" << endl << endl;
	}

	return o;
}