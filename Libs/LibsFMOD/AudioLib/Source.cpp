#pragma once
#include "AudioManager.h"
#include <chrono>
#include<thread>
#include <stdlib.h>
using namespace std;


void init(string file)
{
	//sound->releaseSound(soundSample);
}
int main()
{
	AudioManager* audio = new AudioManager();
	float timeStep = 2;
	float auxTime = timeStep;
	float deltaTime = 0.02;
	//audio->playSound("CorazonPartio", false, 1, CHANNEL::Default);
	audio->playSound("Megalovania", false, 1, CHANNEL::Disparos);
	//audio->changePitch(2, CHANNEL::Default);
	audio->changePitch(1.25, CHANNEL::Disparos);
	while (true)
	{
		this_thread::sleep_for(8000000ns);
		
		if (auxTime > 0)
			auxTime -= deltaTime;
		else
		{
			auxTime = timeStep;
			//audio->playSound("Bruh", false, 1, CHANNEL::Default);
			//audio->playSound("JohnCena", false, 1, CHANNEL::Default);

			cout << "Playing";
			
			//audio->playSound("Bruh", false);
			//audio->playSound("JohnCena", false);
		}

	}
	//system("PAUSE");
	return 0;
}