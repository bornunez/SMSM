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
	meshRend->PlayAnimation("Move", true);
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
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		if (estado == state::IDLE) {
			if (absDist > dist*1.5) {
				rb->setLinearVelocity({ -auxVec.x / 4, 0, -auxVec.z / 4 });
			}
			else if (absDist < dist) {
				meshRend->PlayAnimation("Move", true);
				estado = state::FLEEING;
				rb->clearForces();
			}
		}
		else if (estado == state::FLEEING) {
			auxVec.normalise();
			float angle = atan2(auxVec.x, auxVec.z);
			btQuaternion q;
			q.setX(0);
			q.setY(1 * sin(angle / 2));
			q.setZ(0);
			q.setW(cos(angle / 2));

			rb->getWorldTransform().setRotation(q);
			auxVec *= moveSpeed;
			//aqui se le puede meter un multiplicador random en plan
			//0.8, 1.2 para que haga un poco de s el bicho
			rb->setLinearVelocity({ auxVec.x, 0, auxVec.z});
			if (absDist > dist) {
				estado = state::IDLE;
				rb->clearForces();
				meshRend->StopAnimation(true);
			}
		}
	}
}

void ShyGuy::OnHit() {
	cout << "ONHIT DE SHYGUY" << endl;
	HP--;
	if (HP <= 0) {
		OnDeath();
	}

}

void ShyGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);

	Enemy::OnDeath();
}

void ShyGuy::Spawn()
{
}
