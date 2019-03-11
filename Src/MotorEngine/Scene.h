#pragma once

#include "Game.h"
#include "PhysicsManager.h"

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

	int i = 0;

public:
	Scene(Game* _g);
	void Update();			// Actualiza la escena
	~Scene();
};

