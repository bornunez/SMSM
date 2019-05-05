#pragma once
#include <vector>
#include "Room.h"

class RoomManager
{
private:
	static RoomManager* instance;

	Room* currRoom = nullptr;
	std::vector<Room*> rooms;
	
	Room* AddRoom(int index);
public:
	RoomManager();
	~RoomManager();
	static RoomManager* getInstance();

	Room* GetRoom(int index);
	void SetActiveRoom(Room* r) { currRoom = r; }
	Room* GetActiveRoom() { return currRoom; }	
	void Destroy();
};	 

