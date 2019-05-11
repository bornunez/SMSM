#pragma once
#include "LocalizationManager.h"

class ResourcesManager
{
public:
	static ResourcesManager* GetInstance();

	LocalizationManager* GetLocalizationManager() { return &localizationManager_; }

	void SetRootFolder(string resourcesDirectory) { resourcesDirectory_ = resourcesDirectory; }
	~ResourcesManager();
	void ResetInstance();

private:
	static ResourcesManager *instance;

	ResourcesManager();


	string resourcesDirectory_;

	LocalizationManager localizationManager_;
};

