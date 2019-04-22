#include "Spawner.h"
#include "Room.h"
#include "RoomManager.h"

Spawner::Spawner(GameObject* o) : Component(o)
{
}

Spawner::Spawner(GameObject * o, int _index) : Component(o), index(_index)
{
	//Una vez lo tengamos, nos agregamos a la sala correspondiente
	room = RoomManager::getInstance()->GetRoom(index);
	room->AddSpawner(this);
}


Spawner::~Spawner()
{
}

void Spawner::LoadFromFile(json obj)
{
	//Lo primero es hayar nuestro indice
	if (obj.contains("index"))
		index = obj["index"];
	else
		cout << "ERROR: El spawner tiene que tener un index asignado" << endl;

	//Una vez lo tengamos, nos agregamos a la sala correspondiente
	room = RoomManager::getInstance()->GetRoom(index);
	room->AddSpawner(this);

	////Tambien registramos los que vana ser nuestros posibles enemigos
	//if (obj.contains("enemies")) {
	//	
	//}
}

void Spawner::Spawn()
{

}
