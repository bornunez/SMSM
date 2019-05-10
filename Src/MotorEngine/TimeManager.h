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
	float globalTime = 1;
public:
	static TimeManager* getInstance();
	static void ResetInstance();
	TimeManager();
	~TimeManager();
	void Update();
	float getDeltaTime() { return deltaTime; };
	void setDeltaTime(float time) { deltaTime = time; };
	void setGlobalTime(float globTime) { globalTime = globTime; };
	float getGlobalTime() { return globalTime; };
	int getFPS() { return fps; };
};