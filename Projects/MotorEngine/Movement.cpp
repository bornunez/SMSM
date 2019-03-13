#include "Movement.h"
#include "Component.h"
#include <iostream>

Movement::Movement(GameObject* o) : Component(o)
{
}


Movement::~Movement()
{
}

void Movement::Update()
{
	std::cout << "Paso\n";
	gameObject->getNode()->translate(0.5, 0, 0);
	//gameObject->getNode()->translate(Ogre::Vector3(200, 0, 0));
	cout << "Posicion: " << gameObject->getPosition().x << ", " << gameObject->getPosition().y << ", " << gameObject->getPosition().z << endl;

}
