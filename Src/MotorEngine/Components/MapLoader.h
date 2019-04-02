#pragma once

#include "..\ResourcesManager//JsonParser.h"
class MapLoader : public Component
{
private:
	json mapFile;
	Scene* mapScene;

public:
	MapLoader(GameObject * _gameObject) : Component(_gameObject) {};
	virtual void LoadFromFile(json obj);
	~MapLoader();

	virtual void Awake();

};
