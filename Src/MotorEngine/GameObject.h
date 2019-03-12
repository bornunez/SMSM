#pragma once
#include <list>
#include <OgreSceneNode.h>
#include <OgreEntity.h>


using namespace std;

//"Includes" circulares
class GameObject;
class Component;
class Scene;
class Transform;
class Ogre::SceneManager;


class GameObject
{
private:
	//Listas de cosas que tiene el GO
	std::list<Component*> components;
	std::list<GameObject*> children;
	std::list<Ogre::Entity*> entities; //Entidades de ogre que componen el GameObject

	//Punteros a cosas de arriba
	Scene* scene;
	GameObject* parent;
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* sceneNode;				//Nodo de la escena
	Ogre::SceneNode* parentNode;			//Nodo del padre
	Ogre::SceneNode* mNode;					//Nodo del objeto

	

	Transform* transform;
	string name;
	bool active;

	void OnActive();
	void OnInactive();
public:
	GameObject(Scene* _scene, Ogre::SceneManager* _mSceneManager,string _name, bool _active = true, GameObject* _parent = nullptr);
	~GameObject();
	void ClearComponents();

	void Load();
	void Start();


	//Manejo de entidades
	void AddMesh(string mesh);
	//void AddMesh(string mesh, Vector3 scale);

	//Manejo del transform
	Ogre::Vector3 getPosition();
	void setPosition(Ogre::Vector3 pos);

	//Manejo de Componentes
	std::list<Component*> getComponents() { return components; }
	void RemoveComponent(Component* c);
	void AddComponent(Component* c);
	void BroadcastMessage();

	//Manejo del acivado
	bool isActive() { return active; }
	void SetActive(bool act);


	//GETS Y SETS
	Transform* GetTransform() { return transform; }
	Ogre::SceneNode* getNode() { return mNode; }
	string getName() { return name; }
	void setName(string _name) { name = _name; }
};

