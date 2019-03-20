#pragma once
#include <map>
#include <string>
#include "..\ResourcesManager\JsonParser.h"
#include "..\Scene.h"
#include "..\GameObject.h"
#include "ComponentLoader.h"


using json = nlohmann::json;

class PrefabManager
{
private:
	static PrefabManager* instance;
	std::map<string, json> prefabs;
	string rootFolder;
	//ComponentLoader* componentLoader;

	
public:
	PrefabManager();
	virtual ~PrefabManager();

	static PrefabManager* getInstance();
	void SetRootFolder(string path);

	void LoadAllPrefabs();
	void LoadPrefab(string path);


	GameObject* GetFromPrefab(string prefabName, Scene* scene, GameObject* parent);
};

