#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_common.h"
#include "FMOD/fmod_errors.h"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#endif

//Lleva la gestión de canales y reproducción de sonidos

#pragma once
typedef FMOD::Sound* SoundClass;

enum CHANNEL{Default, Fondo, Disparos, Enemigos};

class SoundSystemClass
{
public:
	// Pointer to the FMOD instance
	FMOD::System *m_pSystem;

	FMOD::Channel* channelPred;
	FMOD::Channel* channel1;
	FMOD::Channel* channel2;
	FMOD::Channel* channel3;

	SoundSystemClass()
	{
		if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
		{
			channelPred = nullptr;
			channel1 = nullptr;
			channel2 = nullptr;
			channel3 = nullptr;
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
		//      m_pSystem->createSound(pFile, FMOD_HARDWARE, 0, pSound);
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
		case CHANNEL::Enemigos:
			channel2->setPitch(velocity);
			break;
		case CHANNEL::Fondo:
			channel3->setPitch(velocity);
			break;
		}
	}
	void playSound(SoundClass pSound, bool bLoop = false, float volume = 1, CHANNEL channel = CHANNEL::Default)
	{


		//MIO NO
		if (!bLoop)
			pSound->setMode(FMOD_LOOP_OFF);
		else
		{
			pSound->setMode(FMOD_LOOP_NORMAL);
			pSound->setLoopCount(-1);
		}

		//MIO
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
		case CHANNEL::Enemigos:
			channel1->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel2);
			channel2->setVolume(volume);
			break;
		case CHANNEL::Fondo:
			channel1->stop();
			m_pSystem->playSound(pSound, NULL, false, &channel3);
			channel3->setVolume(volume);
			break;
		}


		//channel->setPaused(true);

		//       m_pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, 0);
	}

	void releaseSound(SoundClass pSound)
	{
		pSound->release();
	}
};

