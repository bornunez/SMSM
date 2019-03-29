#pragma once
#include "Ogre.h"
class TimeManager
{
private:
	static TimeManager* instance;
	float deltaTime;
	float old;
	Ogre::Timer* Timer;
	int fps;
public:
	static TimeManager* getInstance()
	{
		if (instance == 0)
		{
			instance = new TimeManager();
		}

		return instance;
	};
	TimeManager();
	~TimeManager();
	void Update();
	float getDeltaTime() { return deltaTime; }
	int getFPS() { return fps; };
};