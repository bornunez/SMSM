#include "Spawner.h"
#include "Room.h"
#include "RoomManager.h"
#include "../../../Src/MotorEngine/Scene.h"

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
	else {
#ifdef C_DEBUG
		cout << "ERROR: El spawner tiene que tener un index asignado" << endl;
#endif
	}

	//Una vez lo tengamos, nos agregamos a la sala correspondiente
	room = RoomManager::getInstance()->GetRoom(index);
	room->AddSpawner(this);
}

void Spawner::Spawn()
{
	int rnd = rand() % enemies.size();
	scene->Instantiate(enemies.at(rnd), gameObject->getPosition()+Vector3(0,0.5,0),0.5);
}
