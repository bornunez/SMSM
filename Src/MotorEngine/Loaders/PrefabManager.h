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


	
	GameObject* GenerateGameObject(string prefabName, Scene* scene, GameObject* parent, Vector3 position = { 0,0,0 }, float scale = 1, GameObject * existingObj = nullptr);
	GameObject * GenerateGameObject(json obj, Scene * scene, GameObject * parent, Vector3 position = { 0,0,0 }, float scale = 1, GameObject * existingObj = nullptr);
	GameObject* ParseGameObject(json obj, Scene* scene, GameObject* parent, GameObject * existingObj = nullptr);
	GameObject* Instantiate(string prefab, Scene* scene, GameObject* parent,Vector3 position,float scale, GameObject * existingObj = nullptr);
};

