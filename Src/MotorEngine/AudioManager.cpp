#include "AudioManager.h"



AudioManager::AudioManager()
{
	sound = new SoundSystemClass(100, 1);
	cargaSonidos();	
}


AudioManager::~AudioManager()
{
}

void AudioManager::cargaSonidos()
{
	string path = "C:\\Users\\casad\\Desktop\\Universidad\\Efectos de sonido\\";

	sounds["JohnCena"] = path + "AND HIS NAME IS JOHN CENA.mp3";
	sounds["BaDumTss"] = path + "BA DUM TSS.mp3";
	sounds["Bruh"] = path + "BRUH.mp3";
	sounds["Megalovania"] = path + "Megalovania.wav";
	sounds["CorazonPartio"] = path + "alejandro-sanz-corazon-partio-letra.mp3";
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
	sound->playSound(soundSample, loop, volume, channel);
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
