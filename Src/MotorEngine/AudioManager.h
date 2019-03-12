#pragma once
#include <map>
#include "AudioEngine.h"
using namespace std;

//Gestiona los sonidos posibles y enlaza con AudioEngine
class AudioManager
{
public:
	SoundSystemClass soundSystem;
	std::map<string, string> sounds;
	AudioManager();
	~AudioManager();
	void playSound(string fileName, bool loop, float volume = 1, CHANNEL channel = CHANNEL::Default);
	void play3DSound(string fileName, int relX, int relY, int relZ, bool loop, float volume = 1, CHANNEL channel = CHANNEL::Default);
	void changePitch(float velocity = 1, CHANNEL channel = CHANNEL::Default);
	void changePan(float pan, CHANNEL channel);
	void change3DPosition(int relX, int relY, int relZ, CHANNEL channel = CHANNEL::Default);
private:
	string actualSound = "";
	void getSound(string fileName);
	SoundSystemClass* sound;
	SoundClass soundSample;
	void cargaSonidos();
};

