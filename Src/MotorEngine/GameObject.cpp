#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include <OgreSceneManager.h>
#include "Component.h"

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
	scene->Add(c);
}

void GameObject::BroadcastMessage(string message)
{
	for(Component* c : components)
		if (c->Enabled()){
			c->receiveMessage(message);
		}

}

void GameObject::SetActive(bool act)
{
	//En caso de que nos intenten activar y estuvieramos inactivos, llamamos al OnActive
	if (act && !active)
		OnActive();
	else if (!act && active)
		OnInactive();
}

void GameObject::AddEntity(Ogre::Entity * entity)
{
	mNode->attachObject(entity);
	entities.push_back(entity);
}

void GameObject::AddChild(GameObject * child)
{

	if (child->getParentNode()!= nullptr) {
		child->getParentNode()->removeChild(child->getNode());
	}
	mNode->addChild(child->getNode());
	child->setParentNode(mNode);
	child->setParent(this);
	
	children.push_back(child);
}



