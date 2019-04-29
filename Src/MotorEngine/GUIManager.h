#pragma once

// Clase encargada de llevar los estados de menú, pausa y GUI dentro del juego

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <map>

class GUILoader;

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
	void HideWindow(std::string wndName);
	void ShowWindow(std::string wndName);

	void AddWindow(std::string wndName, std::string frameWindowLook, int posX, int posy, int sizeX, int sizeY, std::string backgroundMatName = " ");
	void FrameWndImage(std::string name, Ogre::Real left = -1.0, Ogre::Real top = 1.0, Ogre::Real right = 1.0, Ogre::Real down = -1.0);						// Side values go from -1 to 1 from the center of the screen
	void CreateButton(std::string stateWnd, std::string buttonName, std::string buttonScheme, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName);

	bool getGameOn() { return gameHUD; }

private:
	static GUIManager * instance_;
	std::map<std::string, void(GUIManager::*)()> functions;
	//std::map<std::string, void(GUILoader::*)()> functions;

	std::map<std::string, CEGUI::FrameWindow*> stateWnds;		// Relaciona un nombre con su ventana ("estado")
	CEGUI::FrameWindow* activeWnd = nullptr;
	CEGUI::FrameWindow* lastWnd = nullptr;
	
	//Menu example
	CEGUI::FrameWindow* menuWnd;
	//Pause example
	CEGUI::FrameWindow* pauseWnd;
	//GameHUD example
	CEGUI::FrameWindow* gameWnd;

	bool gameHUD = false;
	bool pauseHUD = false;
	bool menuHUD = false;

	Ogre::RenderWindow* renderWindow_ = nullptr;
	Game* g_ = nullptr;
	bool gameEnded = false;

	//CEGUI BASE OBJECTS
	CEGUI::WindowManager* wmgr;
	CEGUI::Window* myRoot;
	CEGUI::OgreRenderer* renderer_;

	// Funciones concretas para el videojuego
	void toggleMenu();
	void togglePause();
	void Exit();
	void InitMainScene();

	// Solo temporal para el estilo de creación de botones
	//GUILoader * loader;
};