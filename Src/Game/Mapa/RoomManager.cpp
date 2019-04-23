#include "RoomManager.h"
RoomManager* RoomManager::instance = nullptr;


Room * RoomManager::AddRoom(int index)
{
	Room* r = new Room(index);
	rooms.push_back(r);

	return r;
}

RoomManager::RoomManager()
{
}


RoomManager::~RoomManager()
{
}

RoomManager * RoomManager::getInstance()
{
	if (instance == nullptr)
		instance = new RoomManager();
	return instance;
}

Room * RoomManager::GetRoom(int index)
{
	for (Room* r : rooms) {
		if (r->GetIndex() == index)
			return r;
	}

	return AddRoom(index);
}
