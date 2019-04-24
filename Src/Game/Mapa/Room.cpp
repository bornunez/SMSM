#include "Room.h"


Room::Room(int _index) : index(_index)
{
	
}

Room::~Room()
{
}

void Room::OnEnter()
{
	cout << "Se ha entrado en la sala " << index << endl;
	//Quitamos las entradas
	for (GameObject* g : entries)
		g->Destroy();
	//Cerramos las salidas
	for (GameObject* g : gates)
		g->SetActive(true);

	//Y spawneamos a los enemigos
	for (Spawner* s : spawners)
		s->Spawn();
}

void Room::OnClear()
{
}
