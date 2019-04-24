#pragma once

// Clase encargada de llevar los estados de men�, pausa y GUI dentro del juego

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <map>

class Game;

class GUIManager
{
public:
	static GUIManager * Instance(Ogre::RenderWindow* w = nullptr, Game* g = nullptr);	

	GUIManager(Ogre::RenderWindow* w, Game* g);
	virtual ~GUIManager();
	
	// Para reconocer movimientos y pulsaciones
	void Update();
	void checkMouse();
	void checkKeys();

	void Initialize();	
	void ToggleWindow(std::string wndName);
	void AddWindow(std::string wndName);
	void CreateButton(std::string stateWnd, std::string buttonScheme, std::string image, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName);

	void toggleMenu();
	void togglePause();
	void Exit();
	void InitMainScene();

	//void destroySystem() { CEGUI::OgreRenderer::destroySystem(); }

private:
	static GUIManager * instance_;
	std::map<std::string, void(GUIManager::*)()> functions;

	std::map<std::string, CEGUI::FrameWindow*> stateWnds;		// Relaciona un nombre con su ventana ("estado")
	CEGUI::FrameWindow* activeWnd = nullptr;
	CEGUI::FrameWindow* lastWnd = nullptr;
	
	//
	CEGUI::FrameWindow* menuWnd;

	bool gameHUD = false;

	Ogre::RenderWindow* renderWindow_ = nullptr;
	Game* g_ = nullptr;
	bool gameEnded = false;

	//CEGUI BASE OBJECTS
	CEGUI::WindowManager* wmgr;
	CEGUI::Window* myRoot;
	CEGUI::OgreRenderer* renderer_;
};