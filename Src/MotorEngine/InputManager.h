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
	InputManager(Ogre::RenderWindow* window);
	~InputManager();

	void SetUpOIS();

	//KeyBoard
	bool getKeyDown(OIS::KeyCode key);

	//Mouse Coords
	std::pair<int,int> getMouseCoords();
	float getMouseX();
	float getMouseY();

	//Mouse Buttons
	bool getMLeftButton();
	bool getMRightButton();
	bool getMWheelButton();
	

protected:
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	Ogre::RenderWindow* mWindow;
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
};

