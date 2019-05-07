#include "SpawnOnDestroy.h"
#include "../../../../Src/MotorEngine/Scene.h"

SpawnOnDestroy::~SpawnOnDestroy()
{
}

void SpawnOnDestroy::LoadFromFile(json obj)
{
	if (obj.contains("objects")) {
		json objects = obj["objects"];
		for (int i = 0; i < objects.size(); i++)
			objectsToSpawn.push_back(objects[i]);
	}
}

void SpawnOnDestroy::OnDestroy()
{
	for (string s : objectsToSpawn) {
		scene->Instantiate(s, gameObject->getGlobalPosition());
	}
}
