#pragma once
#include <map>
#include <string>
#include "..\ResourcesManager\JsonParser.h"
#include "..\Scene.h"
#include "..\GameObject.h"
#include "ComponentLoader.h"


class PrefabManager
{
private:
	static PrefabManager* instance;
	std::map<string, json> prefabs;
	string rootFolder;
	ComponentLoader* componentLoader;

	
public:
	PrefabManager();
	virtual ~PrefabManager();

	static PrefabManager* getInstance();

	void Init(string path, ComponentLoader* compLoader);
	void SetRootFolder(string path);

	void LoadAllPrefabs();
	void LoadPrefab(string path);


	GameObject* GenerateGameObject(string prefabName, Scene* scene, GameObject* parent);
	GameObject* GenerateGameObject(json obj, Scene* scene, GameObject* parent);
	GameObject* ParseGameObject(json obj, Scene* scene, GameObject* parent);
	GameObject* Instantiate(json obj, Scene* scene, GameObject* parent);
};

