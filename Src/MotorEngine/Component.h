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
	string name = "";
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
	virtual void OnDestroy() { /*cout << "Destruyendo [ " << "] del objeto [ " << gameObject->getName() << " ]" << endl;*/ };

	bool isActiveAndEnabled();
	bool isAwake() { return awoke; }
	void setAwake() { awoke = true; }
	bool isStarted() { return started; }
	void setStarted() { started = true; }
	bool isEnabled() { return enabled; }
	void Disable();
	void Enable();
	void SetEnabled(bool _enabled);
	void SetName(string _name) { name = _name; }
	string GetName() { return name; }

	// Sistema de mensajes
	virtual void receiveMessage(string message) {};

	//Gets y sets
	GameObject* getGameObject() { return gameObject; }
	Scene * getScene() { return scene; }

	//Get Component
	template<typename T>
	T* getComponent()
	{
		//Compopnent
		for (Component* c : gameObject->getComponents()) {
			T* component = dynamic_cast<T*>(c);
			if (component != nullptr)
				return component;
		}

		//If component not found
		return nullptr;
	};
};
