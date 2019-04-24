#include "ShyGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

ShyGuy::~ShyGuy()
{
}

void ShyGuy::Start() {
	Enemy::Start();

	//Find mesh renderer
	std::list<Component*> comps = gameObject->getComponents();
	bool found = false;
	auto it = comps.begin();
	while (!found && it != comps.end())
	{
		MeshRenderer* c = dynamic_cast<MeshRenderer*>(*it);
		if (c != nullptr) {
			found = true;
			meshRend = c;
		}
		it++;
	}

	meshRend->InitAnimations();
}

void ShyGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setGravity(btVector3(0, obj["gravity"], 0));
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	moveSpeed = obj["moveSpeed"];
	dist = obj["dist"];
}


void ShyGuy::Update()
{
	if (estado != state::DEAD) {
		Ogre::Vector3 auxVec = -(player->getPosition() - gameObject->getPosition());
		float absDist = abs(auxVec.x) + abs(auxVec.y);
		if (estado == state::IDLE) {
			if (absDist < dist)
				meshRend->PlayAnimation("Move", true);
				estado == state::FLEEING;
		}
		else if (estado == state::FLEEING) {
			auxVec.normalise();
			auxVec *= moveSpeed;
			rb->applyCentralImpulse({ auxVec.x, auxVec.y, 0});
			if (absDist > dist) {
				estado == state::IDLE;
				rb->clearForces();
				meshRend->StopAnimation(true);
			}
		}
	}
}

void ShyGuy::OnHit() {
	HP--;
	if (HP <= 0) {
		OnDeath();
	}
}

void ShyGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
}

void ShyGuy::Spawn()
{
}
