#include "MapHandler.h"


MapHandler::~MapHandler()
{
}

void MapHandler::OnDestroy()
{
	RoomManager::getInstance()->Destroy();
}
