#include "GameObject.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include <OgreSceneManager.h>
#include "Component.h"

void GameObject::OnActive()
{
	active = true;
#ifdef C_DEBUG
	cout << "Activado [ " << getName() << " ]" << endl;
#endif
	//Avisamos a todos los componentes activos de que nos hemos activado
	for (Component* c : components) {
		if (c->isEnabled()) {
			if (!c->isStarted()) {
				c->Start();
				c->setStarted();
			}
			else
				c->OnEnable();
		}
	}
}

void GameObject::OnInactive()
{
#ifdef C_DEBUG
	cout << "Desactivado [ " << getName() << " ]" << endl;
#endif
	active = false;
	//Avisamos a todos los componentes activos de que nos desactivamos
	for (Component* c : components) {
		if(c->isEnabled() && c->isStarted())
			c->OnDisable();
	}
}

GameObject::GameObject(Scene * _scene, string _name, bool _active, GameObject * _parent)
	: scene(_scene), mSceneManager(scene->getSceneManager()), name(_name), active(_active), parent(_parent)
{
	//Cogemos el nodo de la escena de la propia escena
	sceneNode = scene->getSceneNode();

	//Y luego configuramos nuestro nodo padre de tal modo que si no existe se pone como hijo de la escena
	if (parent != nullptr) {
		parentNode = parent->getNode();
		mNode = parentNode->createChildSceneNode();
	}
	else {
		parentNode = sceneNode;
		mNode = sceneNode->createChildSceneNode();
	}
}

GameObject::GameObject(Scene * _scene)
	: scene(_scene), mSceneManager(scene->getSceneManager()), name(""), active(true), parent(nullptr)
{
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

Ogre::Vector3 GameObject::getGlobalPosition()
{
	Ogre::Vector3 pos;
	if (parent != nullptr)
		pos = parent->getGlobalPosition() + getPosition();
	else
		pos = scene->getSceneNode()->convertLocalToWorldPosition(getPosition());
	return pos;
}

void GameObject::setPosition(Vector3 pos)
{
	mNode->setPosition(pos);
}

void GameObject::setScale(float scale)
{
	mNode->setScale(scale, scale, scale);
}

Ogre::Quaternion GameObject::getGlobalOrientation()
{
	Ogre::Quaternion dir;
	if (parent != nullptr)
		dir = parent->getGlobalOrientation() + getOrientation();
	else
		dir = getOrientation();
	return dir;
}

Ogre::Quaternion GameObject::getOrientation()
{
	return mNode->getOrientation();
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
		if (c->isEnabled()){
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

void GameObject::AddEntity(Ogre::MovableObject * entity)
{
	auto it = entities.begin();
	while (it != entities.end() && *it != entity)
		it++;
	if (it != entities.end())
		cout << "ERROR: La entidad: " << entity->getName() << " del objeto " << name << " ya ha sido añadida" << endl;
	else {
		mNode->attachObject(entity);
		entities.push_back(entity);
	}
}

void GameObject::RemoveEntity(Ogre::MovableObject * entity)
{
	auto it = entities.begin();
	while (it != entities.end() && *it != entity)
		it++;
	if (it != entities.end()) {
		mNode->detachObject(entity);
		entities.remove(entity);
	}
	else {
		cout << "ERROR: La entidad: " << entity->getName() << " del objeto " << name << " ya ha sido eliminada" << endl;
	}
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

GameObject * GameObject::GetChild(string _name)
{
	for (GameObject* o : children)
		if (o->getName() == _name)
			return o;
	return nullptr;
}

void GameObject::Destroy()
{
	scene->Destroy(this);
}



