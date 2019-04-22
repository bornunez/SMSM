#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "Spawner.h"

class Entry {
	int x, y;
	int width, height;
	Entry(int _x, int _y, int w, int h) : x(_x), y(_y), width(w), height(h) {

	}

};

class Room
{
private:
	int index = -1;
	list<Spawner*> spawners;
	list<GameObject*> gates;
	list<Entry> entries;
public:
	Room(int _index);
	~Room();

	void AddSpawner(Spawner* s) { spawners.push_back(s); };
	void AddEntry(Entry e) { entries.push_back(e); }


	void OnEnter();
	void OnClear();

	int GetIndex() { return index; }

};

