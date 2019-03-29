#pragma once
#include "OgreRenderWindow.h"
#include <OgreFrameListener.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <utility>
//ogre windows event utilities?


class InputManager : public Ogre::FrameListener
{

public:
	/**
		Devuelve una instancia del InputManager
		*/
	static InputManager* getInstance();
	~InputManager();
	/**
		Crea una instancia del InputManager si aun no existe una.
		Recibe la ventana de la aplicacion, en la que captura el input.
		*/
	static void createInstance(Ogre::RenderWindow* window);


	//KeyBoard
	/**
		Indica si una tecla esta siendo pulsada.
		Recibe el OIS::KeyCode correspondiente a la tecla.
		*/
	bool getKeyDown(OIS::KeyCode key);

	/**
		Indica si una tecla acaba de ser pulsada.
		Recibe el OIS::KeyCode correspondiente a la tecla.
		*/
	bool getKeyPressed(OIS::KeyCode key);

	/**
		Indica si una tecla acaba de ser soltada.
		Recibe el OIS::KeyCode correspondiente a la tecla.
		*/
	bool getKeyUp(OIS::KeyCode key);


	//Mouse Coords
	/**
		Devuelve las cordenadas del raton en un pair<float,float>
		*/
	std::pair<int,int> getMouseCoords();

	/**
		Devuelve la cordenada X del raton
		*/
	int getMouseX();

	/**
		Devuelve la cordenada Y del raton
		*/
	int getMouseY();

	//Mouse Buttons
	/**
		Indica si un boton del raton esta siendo pulsado.
		Recibe el OIS::MouseButtonID correspondiente al boton.
		*/
	bool getMouseButtonDown(OIS::MouseButtonID buttonID);

	/**
		Indica si un boton del raton acaba de ser pulsado.
		Recibe el OIS::MouseButtonID correspondiente al boton.
		*/
	bool getMouseButtonPressed(OIS::MouseButtonID buttonID);

	/**
		Indica si un boton del raton acaba de ser soltado.
		Recibe el OIS::MouseButtonID correspondiente al boton.
		*/
	bool getMouseButtonUp(OIS::MouseButtonID buttonID);

protected:
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	Ogre::RenderWindow* mWindow;
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	char prevKeyboard[256];
	OIS::MouseState prevMouse;

	static InputManager* instance;
	InputManager(Ogre::RenderWindow* window);
	void SetUpOIS();

	template<class TYPE> inline TYPE BIT(const TYPE & x)
	{
		return TYPE(1) << x;
	}

	template<class TYPE> inline bool IsBitSet(const TYPE & x, const TYPE & y)
	{
		return 0 != (x & y);
	}
};

