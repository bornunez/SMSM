#pragma once
#include "LocalizationManager.h"

class ResourcesManager
{
public:
	static ResourcesManager* GetInstance();


	LocalizationManager* GetLocalizationManager() { return &localizationManager_; }

	void SetRootFolder(string resourcesDirectory) { resourcesDirectory_ = resourcesDirectory; }

private:
	static ResourcesManager *instance;

	ResourcesManager();
	~ResourcesManager();

	string resourcesDirectory_;

	LocalizationManager localizationManager_;
};

