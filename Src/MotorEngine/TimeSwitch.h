#pragma once
#include "../MotorEngine/TimeManager.h"

class TimeSwitch
{
private:
	bool stopped = false;
	float actTime = 0;
public:
	TimeSwitch();
	~TimeSwitch();
	void update();
	void reset()
	{
		actTime = 0;
	};
	void stop(bool active)
	{
		stopped = active;
	};

	float getTime()
	{
		return actTime;
	}
};



