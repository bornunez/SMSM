#include "EnemyTest.h"

EnemyTest::~EnemyTest()
{
}
void EnemyTest::Start()
{
	tm = TimeManager::getInstance();
}
void EnemyTest::LoadFromFile(json obj)
{
	RigidBodyComponent::LoadFromFile(obj);
	physicRB = PhysicsManager::Instance()->CreateBoxCollider(this, id, gameObject->getNode(), mass, gameObject->getNode()->getPosition().x, gameObject->getNode()->getPosition().y, gameObject->getNode()->getPosition().z, restitutionFactor, sizeX, sizeY, sizeZ, offSetX, offSetY, offSetZ);

	//Params from file
	physicRB->setGravity(btVector3(0, obj["gravity"], 0));
	physicRB->setDamping(obj["linDamp"], obj["angDamp"]);
	moveSpeed = obj["moveSpeed"];
	moveTime = obj["moveTime"];
}

void EnemyTest::Update()
{
	if (!hit) {
		physicRB->applyCentralImpulse(dirs[curDir] * moveSpeed);
		moveTimer += tm->getDeltaTime();
		if (moveTimer >= moveTime) {
			curDir++;
			physicRB->setLinearVelocity(physicRB->getLinearVelocity() / 4);
			if (curDir == dirs.size()) curDir = 0;
			moveTimer = 0;
		}
	}
}

void EnemyTest::collisionHandler(int id)
{
	if(!hit && id == 7)
		hit = true;
	//Comprobar id -> bala
	/*if (id == 0) {
		if (!hit) {
			std::cout << "Fui disparado" << std::endl;
			hit = true;
			physicRB->clearForces();
		}
	}*/
}
