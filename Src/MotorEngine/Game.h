#pragma once

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
#include "OISExample.h"
#include "InputManager.h"


class Scene;
class ComponentLoader;


using namespace Ogre;

class Game
{
private:
	Ogre::Root* mRoot;					//Nodo raiz
	Ogre::String mResourcesCfg;			// Contiene el nombre del archivo resources.cfg
	Ogre::String mPluginsCfg;			// Contiene el nombre del archivo plugins.cfg
	Ogre::RenderWindow* mWindow;		// Ventana del juego
	bool endGame = false;				// Controla cuando termina el juego, sacandolo del bucle principal

	Scene* testScene;					// Escena de prueba para probar funcionalidad

	OISExample* oisex;
	InputManager* mInputM;

	ResourcesManager* resourcesManager;
	ComponentLoader* componentLoader;

	void SetUpResources();				
	void InitWindow();
	void MessagePump();


	
public:
	Game(ComponentLoader* _componentLoader);
	~Game();

	void Play();

	Root* getRoot() { return mRoot; }
	RenderWindow* getRenderWindow() { return mWindow; }
};

