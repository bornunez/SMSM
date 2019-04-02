#include "TimeSwitch.h"


TimeSwitch::TimeSwitch()
{
	actTime = 0;
}


TimeSwitch::~TimeSwitch()
{
}

void TimeSwitch::update()
{
	if (!stopped)
		actTime += TimeManager::getInstance()->getDeltaTime();
}
