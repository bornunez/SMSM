#include "AudioManager.h"
#include "GUIManager.h"

AudioManager* AudioManager::instance = 0;


AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{
	delete sound;
}

void AudioManager::ResetInstance()
{
	delete instance;
	instance = nullptr;
}

void AudioManager::init()
{
	sound = new SoundSystemClass(100, 1);
	cargaSonidos();
}
void AudioManager::cargaSonidos()
{
	string path = ".\\Assets\\Audio\\";

	sounds["GunShoot"] = path + "PISTOL-SHOOT.mp3";
	sounds["ShotGunShoot"] = path + "shotgun.mp3";
	sounds["BandaSonora1"] = path + "DeathMatch (Boss Theme).mp3";
}

void AudioManager::getSound(string fileName)
{
	// Initialize our sound system
	// Create a sample sound
	soundSample = SoundClass();
	string nombre = sounds[fileName];
	const char* name = nombre.c_str();
	sound->createSound(&soundSample, name);
}
void AudioManager::change3DPosition(int x, int y, int z, CHANNEL channel)
{
	sound->change3DPosition(x, y, z, channel);
}
void AudioManager::modifyVolume(bool v)
{
	if (v) {
		globalVolume += 0.2f;
		if (globalVolume > 2.0f)
			globalVolume = 2.0f;
	}
	else {
		globalVolume -= 0.2f;
		if (globalVolume < 0)
			globalVolume = 0;
	}

	GUIManager::Instance()->getButton("Volume")->setText("Volume: " + std::to_string((int)(globalVolume * 100)) + "%");

}
void AudioManager::muteVolume()
{
	if (globalVolume > 0) {
		savedVolume = globalVolume;
		globalVolume = 0;
	}
	else {		
		globalVolume = savedVolume;
	}

	GUIManager::Instance()->getButton("Volume")->setText("Volume: " + std::to_string((int)(globalVolume * 100)) + "%");
}
void AudioManager::changePitch(float velocity, CHANNEL channel)
{
	sound->changePitch(velocity, channel);
}
void AudioManager::changePan(float pan, CHANNEL channel)
{
	sound->changePan(pan, channel);
}
void AudioManager::playSound(string fileName, bool loop, float volume, CHANNEL channel)
{
	if (actualSound != fileName)
	{
		actualSound = fileName;
		getSound(fileName);
	}
	// Play the sound, with loop mode
	sound->playSound(soundSample, loop, volume * globalVolume, channel);
}
void AudioManager::play3DSound(string fileName, int x, int y, int z, bool loop, float volume, CHANNEL channel)
{
	if (actualSound != fileName)
	{
		actualSound = fileName;
		getSound(fileName);
	}
	// Play the sound, with loop mode
	sound->play3DSound(soundSample, x, y, z, loop, volume, channel);
}
