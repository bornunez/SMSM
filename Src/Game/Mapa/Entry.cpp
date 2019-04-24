#include "Entry.h"
#include "RoomManager.h"

Entry::Entry(GameObject * o) : PlayerTrigger(o)
{
}

Entry::Entry(GameObject * o,int _index, int w, int h, int _offset_x, int _offset_y) : PlayerTrigger(o,w,h,_offset_x,_offset_y), index(_index)
{
	Initialize();
}

void Entry::Initialize()
{
	room = RoomManager::getInstance()->GetRoom(index);
	room->AddEntry(gameObject);
}

void Entry::OnTriggerEnter()
{
	room->OnEnter();
}

Entry::~Entry()
{
}
