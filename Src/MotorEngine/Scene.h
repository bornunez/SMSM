#pragma once

#include "Game.h"

class Scene
{
private:
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* sceneNode;
	Game * g;
	//Class Camera
	Ogre::SceneNode* mCamNode = nullptr;
	//Class LightManager
	Ogre::SceneNode* mLightNode = nullptr;
	//Nodo de prueba
	Ogre::SceneNode* testNode = nullptr;



public:
	Scene(Game* _g);
	void Update();			// Actualiza la escena
	~Scene();
};

