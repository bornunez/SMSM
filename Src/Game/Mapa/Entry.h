#pragma once
#include "../Componentes/Player/PlayerTrigger.h"

class Room;

class Entry : public PlayerTrigger
{
private:
	int index = -1;
	Room* room = nullptr;
public:
	Entry(GameObject* o);
	Entry(GameObject* o,int _index,int w, int h, int _offset_x = 0, int _offset_y = 0);

	void Initialize();
	void OnTriggerEnter();

	virtual ~Entry();
};