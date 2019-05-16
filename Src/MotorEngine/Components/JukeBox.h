#pragma once
#include "..\ResourcesManager//JsonParser.h"

using namespace std;
class JukeBox : public Component
{
private:
	std::string song;

public:
	JukeBox(GameObject * _gameObject) : Component(_gameObject) {};
	virtual void LoadFromFile(json obj);
	~JukeBox();

	virtual void Awake();
	virtual void Start();
};