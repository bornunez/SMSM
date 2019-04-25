#include "KnifeGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

KnifeGuy::~KnifeGuy()
{
}

void KnifeGuy::Start() {
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
void KnifeGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setGravity(btVector3(0, obj["gravity"], 0));
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	moveSpeed = obj["moveSpeed"];
}


void KnifeGuy::Update()
{
	if (estado == state::ALIVE) {
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		auxVec.normalise(); 
		auxVec*=moveSpeed;

		rb->applyCentralImpulse({auxVec.x, auxVec.y, 0});
	}
}

void KnifeGuy::OnHit() {
	HP--;
	if (HP <= 0) {
		OnDeath();
	}
	cout << "GOLPE A KNIFE GUY" << endl;
	gameObject->Destroy();
}

void KnifeGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
}

void KnifeGuy::Spawn()
{
}
