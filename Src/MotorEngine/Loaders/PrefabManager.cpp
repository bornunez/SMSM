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

void PrefabManager::ResetInstance()
{
	delete instance; 
	instance = nullptr; 
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
#ifdef C_DEBUG
	cout << endl << endl;
	cout << "=================================================" << endl;
	cout << "========       CARGA DE PREFABS        ==========" << endl;
	cout << "=================================================" << endl << endl;
	cout << "Cagando prefabs de la carpeta: " << rootFolder << endl << endl;
#endif
	std::vector<string> names;
	read_directory(rootFolder, names);
	for (string n : names) {
		LoadPrefab(rootFolder + n);
	}
#ifdef C_DEBUG
	cout << "=================================================" << endl << endl;
#endif
}

void PrefabManager::LoadPrefab(string path)
{
#ifdef C_DEBUG
	cout << "Cargando archivo: " << path << " ..." << endl ;
#endif
	json aux = JsonParser::ParseJsonFile(path);
	prefabs.insert(pair<string,json>(aux["prefabName"], aux));

#ifdef C_DEBUG
	cout << "Cargado prefab con nombre: " << aux["prefabName"] << endl << endl ;
#endif
}

GameObject * PrefabManager::GenerateGameObject(string prefabName, Scene * scene, GameObject * parent, Vector3 position, float scale, GameObject * existingObj )
{
	//Primero intentamos encontrar el json del prefab. Si no existe devolvemos algun tipo de error
	std::map<string,json>::iterator it = prefabs.find(prefabName);
	GameObject* o = nullptr;
	if (it != prefabs.end()) {
		json jsonObj = (*it).second;
#ifdef C_DEBUG
		cout << "Loading prefab [ " << prefabName << " ]" << " as [ " << jsonObj["name"] << " ]" << endl;
#endif
		
		o = GenerateGameObject(jsonObj, scene, parent,position,scale, existingObj);
	}
	else {
#ifdef C_DEBUG
		cout << "ERROR: Prefab [ " << prefabName << " ]" << " doesn't exists" << endl << endl;
#endif
	}

	return o;
}

GameObject * PrefabManager::GenerateGameObject(json obj, Scene * scene, GameObject * parent, Vector3 position, float scale,GameObject* existingObj)
{
	GameObject* o = nullptr;
	string objName = obj["name"];

#ifdef C_DEBUG
	cout << "Trying to load GO [ " << objName << " ]" << endl;
#endif
	
	bool act = true;
	if (obj.contains("active"))
		act = obj["active"];
	if (existingObj == nullptr) {
		o = new GameObject(scene, obj["name"], act, parent);
		scene->Add(o);
	}
	else {
		o = existingObj;
		o->setName(objName);
		o->setParent(parent);
	}

	o->setPosition(position);
	if (obj.contains("scale"))
		scale = obj["scale"];
	o->setScale(scale);

	
	componentLoader->LoadComponents(obj["components"], o);

	//Cargamos tambien todos los hijos
	if (obj.contains("children")) {
#ifdef C_DEBUG
		cout << "Trying to load childs" << endl;
#endif
		for (auto &child : obj["children"])
			if (child.is_object()) {
				GameObject* c = nullptr;
				c = ParseGameObject(child, scene, o);
				if (c != nullptr) {
					o->AddChild(c);
					scene->Add(c);
				}
			}
#ifdef C_DEBUG
		cout << "Succesfully loaded childs" << endl;
#endif
	}

#ifdef C_DEBUG
	cout << "Succesfully loaded [ " << objName << " ]" << endl;
#endif
	return o;

}

GameObject * PrefabManager::ParseGameObject(json obj, Scene * scene, GameObject * parent, GameObject * existingObj)
{
	//Sacamos la posicion del objeto
	Vector3 pos = { 0,0,0 };
	float scale = 1;
	if (obj.contains("position")) {
		json posObj = obj["position"];
		pos = Vector3(posObj["x"], posObj["y"], posObj["z"]);
	}
	if (obj.contains("scale"))
		scale = obj["scale"];

	//Primero tenemos que ver si es un objeto de por si o redirecciona a un prefab
	GameObject* o = nullptr;
	if (obj.contains("prefab")) {
		string prefName = obj["prefab"];
		o = GenerateGameObject(prefName, scene, parent,pos,scale,existingObj);
	}
	else {
		o = GenerateGameObject(obj, scene, parent,pos,scale, existingObj);
	}

	return o;
}

GameObject * PrefabManager::Instantiate(string prefab, Scene * scene, GameObject * parent, Vector3 position, float scale, GameObject * existingObj)
{
	GameObject* o = nullptr;
	o = GenerateGameObject(prefab, scene, parent,position,scale,existingObj);
	
	return o;
}
