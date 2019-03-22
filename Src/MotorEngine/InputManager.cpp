#include "InputManager.h"
#include <OgreException.h>
#include <iostream>

InputManager::InputManager(Ogre::RenderWindow* window)
{
	mWindow = window;
	SetUpOIS();
}


InputManager::~InputManager()
{
	if (mInputManager)
	{
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = 0;
	}
}

void InputManager::SetUpOIS()
{
	std::cout<<("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));

}

bool InputManager::getKeyDown(OIS::KeyCode key)
{
	return mKeyboard->isKeyDown(key);
}

std::pair<int,int> InputManager::getMouseCoords()
{
	std::pair<int,int> mCoord = std::pair<int,int>(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
	return mCoord;
}

float InputManager::getMouseX()
{
	return mMouse->getMouseState().X.abs;
}

float InputManager::getMouseY()
{
	return mMouse->getMouseState().Y.abs;
}

bool InputManager::getMLeftButton()
{
	return mMouse->getMouseState().buttons == 1;
}

bool InputManager::getMRightButton()
{
	return mMouse->getMouseState().buttons == 2;

}

bool InputManager::getMWheelButton()
{
	return mMouse->getMouseState().buttons == 4;
}

bool InputManager::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (mWindow->isClosed())
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	//	return false;

	return true;
	return false;
}
