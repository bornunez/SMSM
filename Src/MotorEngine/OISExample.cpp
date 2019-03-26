#include "OISExample.h"
#include <OgreException.h>
#include <iostream>

OISExample::OISExample(Ogre::RenderWindow* window)
{
	mWindow = window;
	SetUpOIS();
}


OISExample::~OISExample()
{
	if (mInputManager)
	{
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = 0;
	}
}

void OISExample::SetUpOIS() 
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

bool OISExample::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (mWindow->isClosed())
		return false;
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	//std::cout << "Mouse X: "<< mMouse->getMouseState().X.abs << " Y: " << mMouse->getMouseState().Y.abs << std::endl;

	if (mKeyboard->isKeyDown(OIS::KC_A)) std::cout << "A pulsada";

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
	return false;
}
