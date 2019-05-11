#include "creditsManager.h"

creditsManager::~creditsManager()
{
}

void creditsManager::LoadFromFile(json obj)
{
	GUILoader::LoadFromFile(obj);

	scrollSpeed = CEGUI::UDim(obj["scrollSpeed"], obj["scrollSpeed"]);
	timeToGoToMenu = obj["timeToMenu"];

	auto it = Buttons.begin();

	while (it != Buttons.end()) {

		if (it->first != "BackgroundCredits") {
			originalPos.push_back(it->second->getPosition());
		}

		it++;
	}
}


void creditsManager::Update()
{
	if (currentTime >= timeToGoToMenu) {
		GUIManager::Instance()->toggleMenu();
		resetPositions();
	}
	else {
		auto it = Buttons.begin();

		while (it != Buttons.end()) {

			if (it->first != "BackgroundCredits") {
				CEGUI::UVector2 pos;
				pos = it->second->getPosition();
				pos.d_y -= scrollSpeed;
				it->second->setPosition(pos);				
			}

			it++;
		}

		currentTime += TimeManager::getInstance()->getDeltaTime();

		if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_RETURN))
			currentTime = timeToGoToMenu;
	}
}

void creditsManager::resetPositions()
{
	auto it = Buttons.begin();
	int i = 0;

	while (it != Buttons.end()) {

		if (it->first != "BackgroundCredits") {
			it->second->setPosition(originalPos[i]);
			i++;
		}
		it++;		
	}
}
