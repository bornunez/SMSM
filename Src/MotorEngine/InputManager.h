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
	static InputManager* getInstance();
	~InputManager();
	static void createInstance(Ogre::RenderWindow* window);


	//KeyBoard
	bool getKeyDown(OIS::KeyCode key);
	bool getKeyPressed(OIS::KeyCode key);
	bool getKeyUp(OIS::KeyCode key);


	//Mouse Coords
	std::pair<int,int> getMouseCoords();
	float getMouseX();
	float getMouseY();

	//Mouse Buttons
	bool getMouseButtonDown(OIS::MouseButtonID buttonID);
	bool getMouseButtonPressed(OIS::MouseButtonID buttonID);
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

};

