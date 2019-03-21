#pragma once
#include "OgreRenderWindow.h"
#include <OgreFrameListener.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
//ogre windows event utilities?

class OISExample : public Ogre::FrameListener
{
public:
	OISExample(Ogre::RenderWindow* window);
	~OISExample();
	void SetUpOIS();
protected:
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
	Ogre::RenderWindow* mWindow;
	OIS::InputManager* mInputManager;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
};

