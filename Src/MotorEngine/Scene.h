#pragma once

#include "Game.h"
#include "PhysicsManager.h"
#include <list>
#include <queue>
#include "ResourcesManager/JsonParser.h"

using namespace std;

class Component;
class Game;

class Scene
{
private:
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* sceneNode;
	Game * g;
	Ogre::Viewport* vp;
	Camera* cam;

	//Class Camera
	Ogre::SceneNode* mCamNode = nullptr;
	//Class LightManager
	Ogre::SceneNode* mLightNode = nullptr;
	//Nodo de prueba
	Ogre::SceneNode* testNode = nullptr;
	Ogre::SceneNode* secondNode = nullptr;

	//Atributos de la escena
	std::list<GameObject*> gameObjects;
	std::list<Component*> components;
	std::list<GameObject*> listeners;
	queue<GameObject*> trash;

	void parroThings(SceneManager* mSceneManager);
	void ClearTrash();
	int i = 0;

	void LoadFromFile(json sceneFile);

public:
	Scene(Game* _g);
	~Scene();
	void Release();

	void Load(json sceneFile);
	void SetActive(bool active);

	void Start();
	void Update();			// Actualiza la escena
	void Add(GameObject* o);
	void Add(Component* c);
	void Instantiate(GameObject* o);
	void Destroy(GameObject* o);

	//Manejo de Mensajes
	void AddListener(GameObject* o) { listeners.push_back(o); }
	void BroadcastMessage(string message);
	
	//GETS Y SETS
	Ogre::SceneNode* getSceneNode() { return sceneNode; }
	Ogre::SceneManager* getSceneManager() { return mSceneManager; }
	Game* getGame() { return g; };

	GameObject* getGameObject(string name);
};

