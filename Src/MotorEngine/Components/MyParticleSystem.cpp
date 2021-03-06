#include "MyParticleSystem.h"
#include <OgreSceneManager.h>
#include "../TimeManager.h"
#include <OgreParticleSystemManager.h>
#include "../ResourcesManager/ParticleManager.h"


MyParticleSystem::~MyParticleSystem()
{
}

void MyParticleSystem::LoadFromFile(json obj)
{
	if (obj.contains("name"))
		SetName(obj["name"]);
	else {
#ifdef C_DEBUG
		cout << "ERROR: Falta un nombre para el sistema de particulas " << endl;
#endif
	}
	
	if (obj.contains("script"))
		SetScript(obj["script"]);
	else {
#ifdef C_DEBUG
		cout << "ERROR: Falta un script para el sistema de particulas " << endl;
#endif
	}
	if (obj.contains("duration"))
		maxDuration = duration = obj["duration"];
	sys = gameObject->getSceneManager()->createParticleSystem(name,script);
	if (obj.contains("Destroy"))
		destroyOnFinish = obj["Destroy"];
}

void MyParticleSystem::Awake()
{
}

void MyParticleSystem::Start()
{
	gameObject->AddEntity(sys);
	sys->setEmitting(true);
	duration = maxDuration;
}

void MyParticleSystem::Update()
{
	if (maxDuration != -1) {
		duration -= TimeManager::getInstance()->getDeltaTime();
		if (duration <= 0) {
			duration = 0;
			if (destroyOnFinish)
				gameObject->Destroy();
			else
				SetEnabled(false);
		}
	}
}

void MyParticleSystem::OnDisable()
{
	gameObject->RemoveEntity(sys);
	sys->setEmitting(false);
}

void MyParticleSystem::OnEnable()
{
	gameObject->AddEntity(sys);
	sys->setEmitting(true);
	duration = maxDuration;
}

void MyParticleSystem::OnDestroy()
{
	gameObject->RemoveEntity(sys);
	sys->setEmitting(false);
}

void MyParticleSystem::SetName(string _name)
{
	name = ParticleManager::GetInstance()->getName(_name);
}
