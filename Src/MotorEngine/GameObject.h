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
	GameObject(Scene* _scene, string _name, bool _active = true, GameObject* _parent = nullptr);
	GameObject(Scene* _scene);
	~GameObject();
	void ClearComponents();

	void Load();
	void Start();


	//Manejo de entidades

	//void AddMesh(string mesh, Vector3 scale);
	void AddEntity(Ogre::Entity* entity);

	//Manejo del transform
	Ogre::Vector3 getPosition();
	void setPosition(Ogre::Vector3 pos);
	void setScale(float scale);

	//Manejo de Componentes
	std::list<Component*> getComponents() { return components; }
	void RemoveComponent(Component* c);
	void AddComponent(Component* c);
	void BroadcastMessage(string message);

	//Manejo del acivado
	bool isActive() { return active; }
	void SetActive(bool act);

	//Manejo de hijos
	void AddChild(GameObject* child);


	//GETS Y SETS

	Scene* getScene() { return scene; }
	Ogre::SceneManager* getSceneManager() { return mSceneManager; }
	Ogre::SceneNode* getNode() { return mNode; }
	Ogre::SceneNode* getParentNode() { return parentNode; }
	void setParentNode(Ogre::SceneNode* pNode) { parentNode = pNode; }
	GameObject* getParent() { return parent; }
	void setParent(GameObject* newParent) { parent = newParent; }

	string getName() { return name; }
	void setName(string _name) { name = _name; }
};
