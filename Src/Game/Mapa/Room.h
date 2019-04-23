#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "Spawner.h"
#include "Entry.h"

class Room
{
private:
	int index = -1;
	list<Spawner*> spawners;
	list<GameObject*> gates;
	list<GameObject*> entries;
public:
	Room(int _index);
	~Room();

	void AddSpawner(Spawner* s) { spawners.push_back(s); };
	void AddEntry(GameObject* e) { entries.push_back(e); }


	void OnEnter();
	void OnClear();

	int GetIndex() { return index; }

};

