#pragma once
#include "../Components/MyParticleSystem.h"
#include <vector>
class ParticleManager
{
protected:
	static ParticleManager* instance;
	std::map<string, int> registeredNames;
public:
	ParticleManager();
	~ParticleManager();

	static ParticleManager* GetInstance();

	string getName(string originalName);
};

