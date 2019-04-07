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

GameObject * PrefabManager::GenerateGameObject(string prefabName, Scene * scene, GameObject * parent, Vector3 position, float scale)
{
	//Primero intentamos encontrar el json del prefab. Si no existe devolvemos algun tipo de error
	std::map<string,json>::iterator it = prefabs.find(prefabName);
	GameObject* o = nullptr;
	if (it != prefabs.end()) {
		json jsonObj = (*it).second;
		cout << "Loading prefab [ " << prefabName << " ]" << " as [ " << jsonObj["name"] << " ]" << endl;
		o = GenerateGameObject(jsonObj, scene, parent,position,scale);

		//Esto parametros y el hecho de tener o no rigidbody debe leerlos de archivo
		/*RigidBodyComponent * rb = new RigidBodyComponent(o);
		PhysicsManager::Instance()->CreateBoxCollider(rb, 1, o->getNode(), 10, btVector3(o->getPosition().x, o->getPosition().y, o->getPosition().z), btQuaternion(1, 0, 0, 0), 1, btVector3(1, 1, 1));*/
	}
	else {
		cout << "ERROR: Prefab [ " << prefabName << " ]" << " doesn't exists" << endl << endl;
	}

	return o;
}

GameObject * PrefabManager::GenerateGameObject(json obj, Scene * scene, GameObject * parent, Vector3 position, float scale)
{
	GameObject* o = nullptr;
	string objName = obj["name"];
	
	bool act = true;
	if (obj.contains("active"))
		act = obj["active"];
	o = new GameObject(scene, obj["name"], act, parent);

	o->setPosition(position);
	o->setScale(scale);

	auto components = componentLoader->LoadComponents(obj["components"], o);

	//Cargamos tambien todos los hijos
	if (obj.contains("children")) {
		for (auto &child : obj["children"])
			if (child.is_object()) {
				GameObject* c = nullptr;
				c = ParseGameObject(child, scene, o);
				if (c != nullptr) {
					o->AddChild(c);
					scene->Add(c);
				}
			}
	}
	return o;

}

GameObject * PrefabManager::ParseGameObject(json obj, Scene * scene, GameObject * parent)
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
		o = GenerateGameObject(prefName, scene, parent,pos,scale);
	}
	else {
		o = GenerateGameObject(obj, scene, parent,pos,scale);
	}
	if (o != nullptr) {
		scene->Add(o);
		return o;
	}

	return nullptr;
}

GameObject * PrefabManager::Instantiate(string prefab, Scene * scene, GameObject * parent, Vector3 position, float scale)
{
	GameObject* o = nullptr;
	o = GenerateGameObject(prefab, scene, parent,position,scale);
	
	return o;
}
