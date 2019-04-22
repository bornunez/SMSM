#include "Room.h"


Room::Room(int _index) : index(_index)
{
	
}

Room::~Room()
{
}

void Room::OnEnter()
{
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
