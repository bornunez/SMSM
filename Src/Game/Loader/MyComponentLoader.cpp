#include "MyComponentLoader.h"
#include "../Components/ExampleComp.h"
#include "../../../Src/MotorEngine/Components/MyLight.h"
#include "../../../Src/MotorEngine/Components/MyParticleSystem.h"
#include "../Mapa/SkipRoom.h"
#include "../Mapa/MapHandler.h"
#include "../Armas/ShotgunBullet.h"
#include "../../../Src/MotorEngine//Components/JukeBox.h"
#include "../Componentes/ItemRigidBody.h"

MyComponentLoader::MyComponentLoader()
{
}

MyComponentLoader::~MyComponentLoader()
{
}

std::list<Component*> MyComponentLoader::LoadComponents(json json, GameObject * gameObject)
{
	std::list<Component*> components;
	for (auto &comp : json)
		if (comp.is_object()) {
#ifdef C_DEBUG
			cout << "Loading component [ " << comp["type"] << " ]" << endl;
#endif
				Component* c = ParseComponent(comp, gameObject);
				if(c != nullptr)
					components.push_back(c);
		}
#ifdef C_DEBUG
	cout << "Succesfully loaded all components" << endl;
#endif
	return components;
}

Component * MyComponentLoader::ParseComponent(json comp , GameObject * gameObject)
{
	Component* c = nullptr;
	string c_type = comp["type"];

	//Crear instancia del componente en funcion del tipo
	//cout << "Intentando parsear [ " << c_type << " ]" << endl;
	if (c_type == "meshRenderer")
		c = new MeshRenderer(gameObject);	
	if (c_type == "RandomMeshRenderer")
		c = new RandomMeshRenderer(gameObject);
	else if (c_type == "Weapon")
		c = new Weapon(gameObject);
	else if (c_type == "ObstacleRB")
		c = new ObstacleRigidBody(gameObject);
	else if (c_type == "WeaponBullet")
		c = new WeaponBullet(gameObject);
	else if (c_type == "MapLoader")
		c = new MapLoader(gameObject);
	else if (c_type == "Camera")
		c = new MyCamera(gameObject);
	else if (c_type == "Light")
		c = new MyLight(gameObject);
	else if (c_type == "PlayerController")
		c = new PlayerController(gameObject);
	else if (c_type == "PlayerCollision")
		c = new PlayerCollision(gameObject);
	else if (c_type == "testEnemy")
		c = new EnemyTest(gameObject);
	else if (c_type == "EnemyRigidBody")
		c = new EnemyRigidBody(gameObject);
	else if (c_type == "ItemRigidBody")
		c = new ItemRigidBody(gameObject);
	else if (c_type == "KnifeGuy")
		c = new KnifeGuy(gameObject);
	else if (c_type == "ShyGuy")
		c = new ShyGuy(gameObject);
	else if (c_type == "ButterGuy")
		c = new ButterGuy(gameObject);
	else if (c_type == "EnemyBullet")
		c = new EnemyBullet(gameObject);
	else if (c_type == "HouseGuy")
		c = new HouseGuy(gameObject);
	else if (c_type == "BombGuy")
		c = new BombGuy(gameObject);
	else if (c_type == "IncognitoGuy")
		c = new IncognitoGuy(gameObject);
	else if (c_type == "SkipRoom")
		c = new SkipRoom(gameObject);
	else if (c_type == "MapHandler")
		c = new MapHandler(gameObject);
	else if (c_type == "ShotgunBullet")
		c = new ShotgunBullet(gameObject);
	else if (c_type == "JukeBox")
		c = new JukeBox(gameObject);
	else if (c_type == "SpawnOnDestroy")
		c = new SpawnOnDestroy(gameObject);
#ifndef _DEBUG
	else if (c_type == "ParticleSystem")
		c = new MyParticleSystem(gameObject);
	else if (c_type == "GUILoader")
		c = new GUILoader(gameObject);
#endif

	if (c != nullptr) {
		gameObject->AddComponent(c);
		c->LoadFromFile(comp);
		c->SetName(c_type);
	}
#ifdef C_DEBUG
	cout << "Succesfully loaded component [ " << c_type << " ]" << endl;
#endif
	return c;
}
