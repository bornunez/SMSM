#pragma once

#define C_DEBUG

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreFileSystemLayer.h>
#include <OgreTextureManager.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneNode.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <Windows.h>
#include "ResourcesManager/ResourcesManager.h"
#include "InputManager.h"


class Scene;
class ComponentLoader;
class GameSceneManager;

using namespace Ogre;

class Game
{
private:
	Ogre::Root* mRoot;					//Nodo raiz
	Ogre::String mResourcesCfg;			// Contiene el nombre del archivo resources.cfg
	Ogre::String mPluginsCfg;			// Contiene el nombre del archivo plugins.cfg
	Ogre::RenderWindow* mWindow;		// Ventana del juego
	Ogre::Viewport* viewport;			// Viewport de la ventana
	bool endGame = false;				// Controla cuando termina el juego, sacandolo del bucle principal

	InputManager* mInputM;

	GameSceneManager* sceneManager;
	ResourcesManager* resourcesManager;
	ComponentLoader* componentLoader;

	void SetUpResources();				
	void InitWindow();
	void MessagePump();

public:
	Game(ComponentLoader* _componentLoader);
	~Game();

	void Play();
	void Quit();
	void ReLoadScene(std::string scene);
	void ChangeScene(std::string scene);
	Scene* getActiveScene();
	

	Root* getRoot() { return mRoot; }
	RenderWindow* getRenderWindow() { return mWindow; }
	Ogre::Viewport* getViewport() { return viewport; }
	InputManager* getInputManager() { return mInputM; }
};

