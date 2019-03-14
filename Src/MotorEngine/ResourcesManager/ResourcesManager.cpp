#include "ResourcesManager.h"


ResourcesManager* ResourcesManager::instance = 0;

ResourcesManager* ResourcesManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new ResourcesManager();
	}

	return instance;
}


ResourcesManager::ResourcesManager() : localizationManager_(LocalizationManager())
{
}


ResourcesManager::~ResourcesManager()
{
}
