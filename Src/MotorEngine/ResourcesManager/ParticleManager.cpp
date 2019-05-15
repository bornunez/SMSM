#include "ParticleManager.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
}

ParticleManager * ParticleManager::GetInstance()
{
	if (instance == nullptr)
		instance = new ParticleManager();
	return instance;
}

void ParticleManager::ResetInstance()
{
	delete instance;
	instance = nullptr;
}

string ParticleManager::getName(string originalName)
{
	string name = originalName +  std::to_string(registeredNames[originalName]);
	registeredNames[originalName]++;

	return name;
}