#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "Spawner.h"

class Room
{
private:
	int index = -1;
	list<Spawner*> spawners;
	list<GameObject*> gates;
public:
	Room(int _index);
	~Room();

	void AddSpawner(Spawner* s) { spawners.push_back(s); };


	void OnEnter();
	void OnClear();


	int GetIndex() { return index; }

};

