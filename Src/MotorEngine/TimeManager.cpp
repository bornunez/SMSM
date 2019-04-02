#include "TimeManager.h"
#include <iostream>
TimeManager* TimeManager::instance = 0;

TimeManager::TimeManager()
{
	Timer = new Ogre::Timer();
	Timer->reset();
	old = Timer->getMilliseconds();

}

void TimeManager::Update()
{
	deltaTime = (Timer->getMilliseconds() - old)*0.001f;
	old = Timer->getMilliseconds();
	fps = 1.0 / deltaTime;
	//std::cout << fps;
}

TimeManager::~TimeManager()
{
}