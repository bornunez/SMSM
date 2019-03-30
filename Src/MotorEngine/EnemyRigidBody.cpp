#include "EnemyRigidBody.h"
#include <iostream>

void EnemyRigidBody::collisionHandler(int id)
{
	std::cout << "Ha colisionado conmigo un objeto con el identificador: " + id << std::endl;
}

EnemyRigidBody::~EnemyRigidBody()
{
}
