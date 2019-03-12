#pragma once
#include <map>
#include "AudioEngine.h"
using namespace std;

//Gestiona los sonidos posibles y enlaza con AudioEngine
class AudioManager
{
public:
	SoundSystemClass soundSystem;
	map<string, string> sounds;
	AudioManager();
	~AudioManager();
	void playSound(string fileName, bool loop, float volume = 1, CHANNEL channel = CHANNEL::Default);
	void changePitch(float velocity = 1, CHANNEL channel = CHANNEL::Default);
private:
	string actualSound = "";
	void getSound(string fileName);
	SoundSystemClass* sound;
	SoundClass soundSample;
	void cargaSonidos();
};

