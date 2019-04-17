#pragma once
#include "../../../Src/MotorEngine/Component.h"

class Room;

class Spawner : public Component
{
private:
	Room* room;
	int index = -1;
	vector<string> enemies;
public:
	Spawner(GameObject* o);
	~Spawner();
	void LoadFromFile(json obj);

	void Spawn();
};

