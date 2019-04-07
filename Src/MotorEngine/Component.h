#pragma once
#include <list>
#include "GameObject.h"
#include <string>
#include "./ResourcesManager/JsonParser.h"

#include <iostream>
using namespace std;

//"Includes" circulares
class Transform;
class Scene;

class Component
{
protected:
	GameObject * gameObject;
	Scene* scene;
	bool enabled;
	bool started = false;
	bool awoke = false;
public:
	Component(GameObject* _gameObject, bool _enabled = true);
	virtual void LoadFromFile(json obj);
	~Component();

	//Flujo de vida del componente. Estos metodos se implementan en el juego
	virtual void Awake() { awoke = true; };
	virtual void Start() { started = true; };
	virtual void Update() { };
	virtual void LateUpdate() {};

	virtual void OnDisable() {};
	virtual void OnEnable() {};
	virtual void OnDestroy() {};

	bool isActiveAndEnabled();
	bool isAwake() { return awoke; }
	void setAwake() { awoke = true; }
	bool isStarted() { return started; }
	void setStarted() { started = true; }
	bool isEnabled() { return enabled; }
	void Disable();
	void Enable();
	void SetEnabled(bool _enabled);

	// Sistema de mensajes
	virtual void receiveMessage(string message) {};

	//Gets y sets
	GameObject* getGameObject() { return gameObject; }
	Scene * getScene() { return scene; }
};
