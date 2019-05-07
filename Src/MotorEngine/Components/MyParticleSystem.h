#pragma once
#include "../Component.h"
#include <OgreParticleSystem.h>

class MyParticleSystem : public Component
{
protected:
	string name;
	string script;
	float duration = -1; //Duracion de la emision, -1 es infinito
	float maxDuration = -1; //Duracion de la emision, -1 es infinito
	Ogre::ParticleSystem* sys = nullptr;
	bool destroyOnFinish = false;
public:
	MyParticleSystem(GameObject* o) : Component(o) {};
	~MyParticleSystem();
	virtual void LoadFromFile(json obj);

	//Flujo de vida del componente. Estos metodos se implementan en el juego
	void Awake();
	void Start();
	void Update();

	void OnDisable();
	void OnEnable();
	void OnDestroy();


	void SetName(string _name);
	void SetScript(string _script) { script = _script; }
};

