#pragma once
#include <list>
#include "GameObject.h"

#include <iostream>
using namespace std;

//"Includes" circulares
class Transform;

class Component
{
protected:
	GameObject * gameObject;
	Transform* transform;
	bool enabled;
public:
	Component(GameObject* _gameObject,bool _enabled = true);
	~Component();

	//Flujo de vida del componente. Estos metodos se implementan en el juego
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() { cout << "Update: " << gameObject->getName() << endl; };
	virtual void LateUpdate() {};

	virtual void OnDisable() {};
	virtual void OnEnable() {};
	virtual void OnDestroy() {};

	bool isActiveAndEnabled();
	bool Enabled() { return enabled; }
	void Disable();
	void Enable();
	void SetEnabled(bool _enabled);

	//Gets y sets
	GameObject* getGameObject() { return gameObject; }

};

