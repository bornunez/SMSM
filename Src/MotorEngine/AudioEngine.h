#pragma once
#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_
#include "fmod.hpp"
#include "fmod_common.h"
#include "fmod_errors.h"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <math.h>
#endif

//Lleva la gestión de canales y reproducción de sonidos

#define PI 3.14159265359
typedef FMOD::Sound* SoundClass;

enum CHANNEL{Default, Fondo, Disparos, Disparos2, Enemigos};

class SoundSystemClass
{
private:
	float maxHearingDistance;
	float distanceMultiplicator;
	float volumen3D;
	float pan;
public:
	// Pointer to the FMOD instance
	FMOD::System *m_pSystem;

	FMOD::Channel* channelPred;
	FMOD::Channel* channel1;
	FMOD::Channel* channel2;
	FMOD::Channel* channel3;
	FMOD::Channel* channel4;

	SoundSystemClass(float _maxHearingDistance = 100, float _distanceMultiplicator = 1)
	{
		channelPred = nullptr;
		channel1 = nullptr;
		channel2 = nullptr;
		channel3 = nullptr;
		channel4 = nullptr;
		maxHearingDistance = _maxHearingDistance;
		distanceMultiplicator = _distanceMultiplicator;
		if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
		{
			// Report Error
			return;
		}

		int driverCount = 0;
		m_pSystem->getNumDrivers(&driverCount);

		if (driverCount == 0)
		{
			// Report Error
			return;
		}

		// Initialize our Instance with 36 Channels
		m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
	}

	void createSound(SoundClass *pSound, const char* pFile)
	{
		m_pSystem->createSound(pFile, NULL, 0, pSound);
	}
	void changeGlobalMusicVolume(float volume)
	{
		channel4->setVolume(volume);
	}
	void changeGlobalVolume(float volume)
	{
			channelPred->setVolume(volume);

			channel1->setVolume(volume);

			channel2->setVolume(volume);

			channel3->setVolume(volume);

	}
	void changePitch(float velocity = 1, CHANNEL channel = CHANNEL::Default)
	{
		switch (channel)
		{
		case CHANNEL::Default:
			channelPred->setPitch(velocity);
			break;
		case CHANNEL::Disparos:
			channel1->setPitch(velocity);
			break;
		case CHANNEL::Disparos2:
			channel2->setPitch(velocity);
			break;
		case CHANNEL::Enemigos:
			channel3->setPitch(velocity);
			break;
		case CHANNEL::Fondo:
			channel4->setPitch(velocity);
			break;
		}
	}

	void change3DPosition(int relX, int relY, int relZ, CHANNEL channel = CHANNEL::Default, float volume = 1)
	{
		float magnitude = sqrt(pow(relX, 2) + pow(relY, 2) + pow(relZ, 2));
		volumen3D = (1 - (magnitude / maxHearingDistance))*volume;

		float rad = atan2(relZ, relX);
		float angle = rad * (180 / 3.14);
		pan = volume * cos(rad);
		if (pan == 1) pan -= 0.01f;
		if (pan == -1) pan += 0.01f;
		switch (channel)
		{
		case CHANNEL::Default:
			channelPred->setVolume(volumen3D);
			channelPred->setPan(pan);
			break;
		case CHANNEL::Disparos:
			channel1->setVolume(volumen3D);
			channel1->setPan(pan);
			break;
		case CHANNEL::Disparos2:
			channel2->setVolume(volumen3D);
			channel2->setPan(pan);
			break;
		case CHANNEL::Enemigos:
			channel3->setVolume(volumen3D);
			channel3->setPan(pan);
			break;
		case CHANNEL::Fondo:
			channel4->setVolume(volumen3D);
			channel4->setPan(pan);
			break;
		}
	}
	void changePan(float panValue = 1, CHANNEL channel = CHANNEL::Default)
	{
		switch (channel)
		{
		case CHANNEL::Default:
			channelPred->setPan(panValue);
			break;
		case CHANNEL::Disparos:
			channel1->setPan(panValue);
			break;
		case CHANNEL::Disparos2:
			channel2->setPan(panValue);
			break;
		case CHANNEL::Enemigos:
			channel3->setPan(panValue);
			break;
		case CHANNEL::Fondo:
			channel4->setPan(panValue);
			break;
		}
	}
	void playSound(SoundClass pSound, bool bLoop = false, float volume = 1, CHANNEL channel = CHANNEL::Default)
	{
		if (!bLoop)
			pSound->setMode(FMOD_LOOP_OFF);
		else
		{
			pSound->setMode(FMOD_LOOP_NORMAL);
			pSound->setLoopCount(-1);
		}

		switch (channel)
		{
		case CHANNEL::Default:
			channelPred->stop();
			m_pSystem->playSound(pSound, NULL, false, &channelPred);
			channelPred->setVolume(volume);
			break;
		case CHANNEL::Disparos:
			channel1->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel1);
			channel1->setVolume(volume);
			break;
		case CHANNEL::Disparos2:
			channel2->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel2);
			channel2->setVolume(volume);
			break;
		case CHANNEL::Enemigos:
			channel3->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel3);
			channel3->setVolume(volume);
			break;
		case CHANNEL::Fondo:
			channel4->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel4);
			channel4->setVolume(volume);
			break;
		}
	}

	void play3DSound(SoundClass pSound, float relX, float relY, float relZ, bool bLoop = false, float volume = 1, CHANNEL channel = CHANNEL::Default)
	{
		change3DPosition(relX, relY, relZ, channel, volume);

		if (!bLoop)
			pSound->setMode(FMOD_LOOP_OFF);
		else
		{
			pSound->setMode(FMOD_LOOP_NORMAL);
			pSound->setLoopCount(-1);
		}

		switch (channel)
		{
		case CHANNEL::Default:
			channelPred->stop();
			m_pSystem->playSound(pSound, NULL, false, &channelPred);
			channelPred->setVolume(volumen3D);
			channelPred->setPan(pan);
			break;
		case CHANNEL::Disparos:
			channel1->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel1);
			channel1->setVolume(volumen3D);
			channel1->setPan(pan);
			break;
		case CHANNEL::Disparos2:
			channel2->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel2);
			channel2->setVolume(volumen3D);
			channel2->setPan(pan);
			break;
		case CHANNEL::Enemigos:
			channel3->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel3);
			channel3->setVolume(volumen3D);
			channel3->setPan(pan);
			break;
		case CHANNEL::Fondo:
			channel4->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel4);
			channel4->setVolume(volumen3D);
			channel4->setPan(pan);
			break;
		}
	}

	void releaseSound(SoundClass pSound)
	{
		pSound->release();
	}
};

