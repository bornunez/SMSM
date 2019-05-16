#include "creditsManager.h"

creditsManager::~creditsManager()
{
}

void creditsManager::LoadFromFile(json obj)
{
	GUILoader::LoadFromFile(obj);

	//GUIManager::Instance()->setCreditsTime(obj["timeToMenu"]);
	GUIManager::Instance()->setScrollSpeed(obj["scrollSpeed"]);

}
