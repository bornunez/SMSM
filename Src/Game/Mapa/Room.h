#pragma once
#include "../../../Src/MotorEngine/Component.h"
#include "Spawner.h"
#include "Entry.h"

class Room
{
private:
	int index = -1;
	bool cleared = false;
	int nEnemies = 0;
	std::list<Spawner*> spawners;
	std::list<GameObject*> gates;
	std::list<GameObject*> entries;
public:
	Room(int _index);
	~Room();

	void AddSpawner(Spawner* s) { spawners.push_back(s); };
	void AddEntry(GameObject* e) { entries.push_back(e); }
	void AddGate(GameObject* o) { gates.push_back(o); }
	void AddEnemy() { nEnemies++; }
	void RemoveEnemies() { nEnemies--; cout << "Enemigos en la sala: " << nEnemies << endl; if (nEnemies <= 0) OnClear(); }

	void OnEnter();
	void OnClear();

	int GetIndex() { return index; }

};