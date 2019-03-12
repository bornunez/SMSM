#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include <OgreSceneManager.h>


void GameObject::OnActive()
{
	//Avisamos a todos los componentes activos de que nos hemos activado
	for (Component* c : components) {
		c->OnEnable();
	}
}

void GameObject::OnInactive()
{
	//Avisamos a todos los componentes activos de que nos desactivamos
	for (Component* c : components) {
		c->OnDisable();
	}
}

GameObject::GameObject(Scene * _scene, Ogre::SceneManager * _mSceneManager, string _name, bool _active, GameObject * _parent)
	: scene(_scene), mSceneManager(_mSceneManager), name(_name), active(_active), parent(_parent)
{
	//Cogemos el nodo de la escena de la propia escena
	sceneNode = scene->getSceneNode();

	//Y luego configuramos nuestro nodo padre de tal modo que si no existe se pone como hijo de la escena
	if (parent != nullptr)
		parentNode = parent->getNode();
	else
		parentNode = sceneNode;

	mNode = sceneNode->createChildSceneNode();


}

GameObject::~GameObject()
{
}

void GameObject::ClearComponents()
{
	for (Component* c : components)
		components.remove(c);
}

// Crea una nueva entidad y la une al nodo al que esta asignado el GameObject
void GameObject::AddMesh(string mesh)
{
	Ogre::Entity* aux = mSceneManager->createEntity(mesh);

	mNode->attachObject(aux);

	entities.push_back(aux);
}

Vector3 GameObject::getPosition()
{
	return mNode->getPosition();
}

void GameObject::setPosition(Vector3 pos)
{
	mNode->setPosition(pos);
}

void GameObject::RemoveComponent(Component * c)
{
	components.remove(c);
}

void GameObject::AddComponent(Component * c)
{
	components.push_back(c);

}

void GameObject::SetActive(bool act)
{
	//En caso de que nos intenten activar y estuvieramos inactivos, llamamos al OnActive
	if (act && !active)
		OnActive();
	else if (!act && active)
		OnInactive();
}


