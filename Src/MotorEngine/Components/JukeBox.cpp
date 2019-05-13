#include "..\Component.h"
#include "JukeBox.h"
#include "..\Loaders\PrefabManager.h"
#include "../../../Src/Game/Mapa/Spawner.h"
#include "../../../Src/Game/Mapa/Entry.h"
#include "../../../Src/Game/Mapa/RoomManager.h"
#include "../AudioManager.h"


JukeBox::~JukeBox()
{
}

void JukeBox::Awake()
{
}

void JukeBox::Start()
{
	AudioManager::getInstance()->playSound(song, true, 0.2, CHANNEL::Fondo);
}

void JukeBox::LoadFromFile(json obj)
{
	//Init
	string songaux = obj["song"];
	song = songaux;
}