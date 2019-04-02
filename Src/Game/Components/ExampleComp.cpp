#include "ExampleComp.h"
#include "../../../Src/MotorEngine/Scene.h"

void ExampleComp::LoadFromFile(json obj)
{
}

void ExampleComp::Awake()
{
	GameObject* player = scene->getGameObject("player");
	player->getComponents();
}


ExampleComp::~ExampleComp()
{
}
