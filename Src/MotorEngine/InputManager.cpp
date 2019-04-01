#include "InputManager.h"
#include <OgreException.h>
#include <iostream>
#include <math.h>

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(Ogre::RenderWindow * window)
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

InputManager * InputManager::getInstance()
{
	return instance;
}

void InputManager::CreateInstance(Ogre::RenderWindow * window)
{
	if (instance == nullptr) {
		instance = new InputManager(window);
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
	mMouse->getMouseState().height = mWindow->getHeight();
	mMouse->getMouseState().width = mWindow->getWidth();
}

bool InputManager::getKeyDown(OIS::KeyCode key)
{
	return (!prevKeyboard[key] && mKeyboard->isKeyDown(key));
}

bool InputManager::getKey(OIS::KeyCode key)
{
	return mKeyboard->isKeyDown(key);
}

bool InputManager::getKeyUp(OIS::KeyCode key)
{
	return (prevKeyboard[key] && !mKeyboard->isKeyDown(key));
}

std::pair<int,int> InputManager::getMouseCoords()
{
	std::pair<int,int> mCoord = std::pair<int,int>(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
	return mCoord;
}

int InputManager::getMouseX()
{
	return mMouse->getMouseState().X.abs;
}

int InputManager::getMouseY()
{
	return mMouse->getMouseState().Y.abs;
}

void InputManager::setMouseCoords(int x, int y)
{
	OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(mMouse->getMouseState());
	mutableMouseState.X.abs = x;
	mutableMouseState.Y.abs = y;
}

void InputManager::CenterMouse()
{
	setMouseCoords(mWindow->getWidth() / 2, mWindow->getHeight() / 2);
}

bool InputManager::getMouseButtonDown(OIS::MouseButtonID buttonID)
{
	return (!IsBitSet(prevMouse.buttons, BIT((int)buttonID)) && IsBitSet(mMouse->getMouseState().buttons, BIT((int)buttonID)));
}

bool InputManager::getMouseButton(OIS::MouseButtonID buttonID)
{
	return IsBitSet(mMouse->getMouseState().buttons, BIT((int)buttonID));
}

bool InputManager::getMouseButtonUp(OIS::MouseButtonID buttonID)
{
	return (IsBitSet(prevMouse.buttons, BIT((int)buttonID)) && !IsBitSet(mMouse->getMouseState().buttons, BIT((int)buttonID)));
}


bool InputManager::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (mWindow->isClosed())
		return false;

	mKeyboard->copyKeyStates(prevKeyboard);
	prevMouse = mMouse->getMouseState();


	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	//	return false;

	return true;
	return false;
}
