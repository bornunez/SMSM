#include "SkipRoom.h"


SkipRoom::~SkipRoom()
{
}

void SkipRoom::LoadFromFile(json obj)
{
}

void SkipRoom::Update()
{
	if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_P)) {
		Room* r = RoomManager::getInstance()->GetActiveRoom();
		if (r != nullptr) {
			r->OnClear();
		}
	}
}