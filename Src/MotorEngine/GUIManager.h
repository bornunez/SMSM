#pragma once

// Clase encargada de llevar los estados de menú, pausa y GUI dentro del juego

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <map>
#include <iostream>

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

	CEGUI::FrameWindow * AddWindow(std::string wndName, std::string frameWindowLook, float posX, float posy, float sizeX, float sizeY, std::string backgroundMatName = " ");
	void FrameWndImage(std::string name, Ogre::Real left = -1.0, Ogre::Real top = 1.0, Ogre::Real right = 1.0, Ogre::Real down = -1.0);						// Side values go from -1 to 1 from the center of the screen
	CEGUI::Window* CreateButton(std::string stateWnd, std::string buttonName, std::string buttonScheme, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName);

	bool getGameOn() { return gameHUD; }
	bool getPauseOn() { return pauseHUD; }
	bool getMenuOn() { return menuHUD; }

	// Devuelve una ventana / un boton segun un nombre dado
	CEGUI::FrameWindow * getWindow(std::string windowName) { return mWindows[windowName]; };
	CEGUI::Window * getButton(std::string buttonName) { return mButtons[buttonName]; };

private:
	static GUIManager * instance_;
	std::map<std::string, void(GUIManager::*)()> functions;

	std::map<std::string, CEGUI::FrameWindow*> stateWnds;		// Relaciona un nombre con su ventana ("estado")
	CEGUI::FrameWindow* activeWnd = nullptr;
	CEGUI::FrameWindow* lastWnd = nullptr;

	// Ventanas y botones
	std::map<std::string, CEGUI::FrameWindow *> mWindows;
	std::map<std::string, CEGUI::Window *> mButtons;

	bool gameHUD = false;
	bool pauseHUD = false;
	bool menuHUD = true;		// Se comienza en el menu

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
	void VolumeUp();
	void VolumeDown();
	void Mute();

	// Solo temporal para el estilo de creación de botones
	//GUILoader * loader;
};