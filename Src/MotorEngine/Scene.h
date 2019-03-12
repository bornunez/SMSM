#pragma once

#include "Game.h"
#include "PhysicsManager.h"
#include <list>
#include <queue>

using namespace std;

class GameObject;
class Component;

class Scene
{
private:
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* sceneNode;
	Game * g;
	PhysicsManager* phyManager;

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
	queue<GameObject*> trash;

	void ClearTrash();

	int i = 0;

public:
	Scene(Game* _g);
	~Scene();
	void Load();
	void Start();
	void Update();			// Actualiza la escena
	void BroadcastMessage();
	void Add(GameObject* o);
	void Add(Component* c);
	void Instantiate(GameObject* o);
	void Destroy(GameObject* o);

	//GETS Y SETS
	Ogre::SceneNode* getSceneNode() { return sceneNode; }
};

