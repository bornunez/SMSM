#pragma once

// Clase encargada de llevar los estados de men�, pausa y GUI dentro del juego

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <map>
#include <vector>
#include <iostream>

class GUILoader;

class Game;

class GUIManager
{
public:
	static GUIManager * Instance(Ogre::RenderWindow* w = nullptr, Game* g = nullptr);	
	static void ResetInstance();

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
	CEGUI::Window* CreateButton(std::string stateWnd, std::string buttonName, std::string buttonScheme, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName, bool gameHUD = false);
	
	CEGUI::Window* CreateLifeIcon(std::string buttonName, float pos_x, float pos_y, float size_x, float size_y);

	bool getGameOn()		{ return gameHUD; }
	bool getPauseOn()		{ return pauseHUD; }
	bool getMenuOn()		{ return menuHUD; }
	bool getGameOverOn()	{ return gameOverHUD; }
	bool getCreditsOn()		{ return creditsHUD; }
	
	void setScrollSpeed(float v) { scrollSpeed = CEGUI::UDim(v,v); fastScrollSpeed = CEGUI::UDim(v*4, v*4);}

	void GameOver();

	// Devuelve una ventana / un boton segun un nombre dado
	CEGUI::FrameWindow * getWindow(std::string windowName) { return mWindows[windowName]; };
	CEGUI::Window * getButton(std::string buttonName) { return mButtons[buttonName]; };

	// Funciones concretas para el videojuego
	void toggleMenu();
	void waitToCredits();

private:
	static GUIManager * instance_;
	std::map<std::string, void(GUIManager::*)()> functions;

	std::map<std::string, CEGUI::FrameWindow*> stateWnds;		// Relaciona un nombre con su ventana ("estado")
	CEGUI::FrameWindow* activeWnd = nullptr;
	CEGUI::FrameWindow* lastWnd = nullptr;

	// Ventanas y botones
	std::map<std::string, CEGUI::FrameWindow *> mWindows;
	std::map<std::string, CEGUI::Window *> mButtons;

	bool gameHUD		= false;
	bool pauseHUD		= false;
	bool menuHUD		= true;		// Se comienza en el menu
	bool gameOverHUD	= false;
	bool creditsHUD		= false;


	//CREDITOS

		std::map<std::string, CEGUI::Window *> creditsElements;
		std::map<std::string, CEGUI::UVector2> originalPos;
		CEGUI::Window * lastCreditsElement = nullptr;
		float timeToCredits = 5;
		CEGUI::UDim scrollSpeed = CEGUI::UDim(0.01, 0.01);
		CEGUI::UDim fastScrollSpeed = CEGUI::UDim(0.01, 0.01);
		bool creditsFinished = false;


		float currentTime = 0;
		float currentWaitTime = 0;
		
		bool waitingToCredits = false;
		void creditsAnim();
		void resetPositions();


	Ogre::RenderWindow* renderWindow_ = nullptr;
	Game* g_ = nullptr;
	bool gameEnded = false;

	//CEGUI BASE OBJECTS
	CEGUI::WindowManager* wmgr;
	CEGUI::Window* myRoot;
	CEGUI::OgreRenderer* renderer_;

	// Funciones concretas para el videojuego
	void toggleCredits();
	void togglePause();
	void Exit();
	void InitMainScene();
	void RestartMainScene();
	void VolumeUp();
	void VolumeDown();
	void VolumeMusicUp();
	void VolumeMusicDown();
	void Mute();
	void MuteMusic();
	void SensitivityUp();
	void SensitivityDown();

	std::vector<CEGUI::Window*> gameHUDelements;
};