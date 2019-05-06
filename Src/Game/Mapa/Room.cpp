#include "Room.h"
#include "RoomManager.h"

Room::Room(int _index) : index(_index)
{
	
}

Room::~Room()
{
}

void Room::OnEnter()
{
#ifdef C_DEBUG
	cout << "Se ha entrado en la sala " << index << endl;
#endif
	RoomManager::getInstance()->SetActiveRoom(this);
	//Quitamos las entradas
	for (GameObject* g : entries) {
		g->Destroy();
	}
	entries.clear();
	//Cerramos las salidas
	for (GameObject* g : gates)
		g->SetActive(true);

	//Y spawneamos a los enemigos
	for (Spawner* s : spawners)
		s->Spawn();
}

void Room::OnClear()
{

	if (!cleared) {
#ifdef C_DEBUG
		cout << "Se ha salido de la sala " << index << endl;
#endif

		//Quitamos las entradas
		for (GameObject* g : entries)
			g->Destroy();
		//Cerramos las salidas
		for (GameObject* g : gates)
			g->Destroy();

		//Y spawneamos a los enemigos
		for (Spawner* s : spawners)
			s->getGameObject()->Destroy();

		cleared = true;
	}
}
