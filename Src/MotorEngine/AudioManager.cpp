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
	setVolumes();
}
void AudioManager::cargaSonidos()
{
	string path = ".\\Assets\\Audio\\";

	//MUSIC
	sounds["BandaSonora1"] = path + "DeathMatch (Boss Theme).mp3";
	sounds["BandaSonora2"] = path + "Alone Against Enemy.mp3";


	//WEAPONS
	sounds["GunShoot"] = path + "PISTOL-SHOOT.mp3";
	sounds["ShotGunShoot"] = path + "shotgun.mp3";

	//SHOUTS
	sounds["ShyGuyShout"] = path + "chillido1.mp3";
	sounds["PocholoShout"] = path + "chillido2.mp3";
	sounds["ButterShout"] = path + "chillido3.mp3";
	sounds["HouseShout"] = path + "chillido4.mp3";
	sounds["IncognitoShout"] = path + "chillido5.mp3";
	sounds["BombExplode"] = path + "bomb.mp3";

	sounds["BossShout"] = path + "boss_shout.mp3";
	sounds["TimeSound"] = path + "time_sound.mp3";
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

void AudioManager::setVolumes()
{
	sound->changeGlobalVolume(globalEffectVolume);
	sound->changeGlobalMusicVolume(globalMusicVolume);
}
void AudioManager::modifyVolume(bool v)
{
	if (v) {
		globalEffectVolume += 0.2f;
		if (globalEffectVolume > 2.0f)
			globalEffectVolume = 2.0f;
	}
	else {
		globalEffectVolume -= 0.2f;
		if (globalEffectVolume < 0)
			globalEffectVolume = 0;
	}
	sound->changeGlobalVolume(globalEffectVolume);

	GUIManager::Instance()->getButton("Volume")->setText("SFX: " + std::to_string((int)(globalEffectVolume * 100)) + "%");
}
void AudioManager::modifyMusicVolume(bool v)
{
	if (v) {
		globalMusicVolume += 0.2f;
		if (globalMusicVolume > 2.0f)
			globalMusicVolume = 2.0f;
	}
	else {
		globalMusicVolume -= 0.2f;
		if (globalMusicVolume < 0)
			globalMusicVolume = 0;
	}
	sound->changeGlobalMusicVolume(globalMusicVolume);

	GUIManager::Instance()->getButton("VolumeMusic")->setText("Music: " + std::to_string((int)(globalMusicVolume * 100)) + "%");
}
void AudioManager::muteMusicVolume()
{
	if (globalMusicVolume > 0) {
		savedMusicVolume = globalMusicVolume;
		globalMusicVolume = 0;
	}
	else {
		globalMusicVolume = savedMusicVolume;
	}
	sound->changeGlobalMusicVolume(globalMusicVolume);
	GUIManager::Instance()->getButton("VolumeMusic")->setText("Music: " + std::to_string((int)(globalMusicVolume * 100)) + "%");
}
void AudioManager::muteVolume()
{
	if (globalEffectVolume > 0) {
		savedVolume = globalEffectVolume;
		globalEffectVolume = 0;
	}
	else {		
		globalEffectVolume = savedVolume;
	}
	sound->changeGlobalVolume(globalEffectVolume);
	GUIManager::Instance()->getButton("Volume")->setText("SFX: " + std::to_string((int)(globalEffectVolume * 100)) + "%");
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
	sound->playSound(soundSample, loop, volume * globalEffectVolume, channel);
}
void AudioManager::play3DSound(string fileName, int x, int y, int z, bool loop, float volume, CHANNEL channel)
{
	if (actualSound != fileName)
	{
		actualSound = fileName;
		getSound(fileName);
	}
	// Play the sound, with loop mode
	sound->play3DSound(soundSample, x, y, z, loop, volume*globalEffectVolume, channel);
}
