#include "ButterGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

ButterGuy::~ButterGuy()
{
}

void ButterGuy::Start() {
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

	tm = TimeManager::getInstance();
}

void ButterGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setGravity(btVector3(0, obj["gravity"], 0));
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	moveSpeed = obj["moveSpeed"];
	dist = obj["dist"];
	distFactor = obj["distFactor"];
	shootSpeed = obj["shootSpeed"];
	shootTime = obj["shootTime"];
	dispersion = obj["dispersion"];
	shootPosX = obj["shootPosX"];
	shootPosY = obj["shootPosY"];
	shootPosZ = obj["shootPosZ"];
	Enemy::alive = true;
}


void ButterGuy::Update()
{
	if (estado != state::DEAD) {
		Ogre::Vector3 auxVec = player->getPosition() - gameObject->getPosition();
		float absDist = abs(auxVec.x) + abs(auxVec.z);
		//No esta tan cerca como para tener que alejarse
		if (estado == state::IDLE) {
			//Si esta demasiado lejos se acerca hasta entrar en el rango de disparo
			if (absDist > dist*distFactor) {
				rb->setLinearVelocity({ auxVec.x, 0, auxVec.z});
			}
			//Si esta demasiado cerca no puede disparar, pasa a huir
			else if (absDist < dist) {
				estado = state::FLEEING;
				rb->setLinearVelocity(btVector3(0, 0, 0));
			}
			//Si esta en rango de disparo
			else {
				estado = state::AIMING;
				rb->setLinearVelocity(btVector3(0, 0, 0));
				shootTimer = 0;
			//	meshRend->StopAnimation(true);
			}
		}
		//si esta huyendo...
		else if (estado == state::FLEEING) {
			auxVec = -auxVec;
			auxVec.normalise();
			auxVec *= moveSpeed;
			rb->setLinearVelocity({ auxVec.x, 0, auxVec.z});
			if (absDist > dist) {
				estado = state::IDLE;
				rb->setLinearVelocity(btVector3(0, 0, 0));
				//meshRend->StopAnimation(true);
			}
		}
		//Si esta apuntando
		else if (estado == state::AIMING) {
			//Dispara cada cierto tiempo
			//Crear bala con direccion auxvec
			shootTimer += tm->getDeltaTime();
			if (shootTimer >= shootTime) {
#ifdef C_DEBUG
				cout << "TE DISPARO CATAPUM CHIMP UM" << endl;
#endif
				Shoot();
				shootTimer = 0;
			}
			//Si deja de estar en rango vuelve a estar idle
			if (absDist < dist || absDist > dist*distFactor) {
				estado = state::IDLE;
				//meshRend->PlayAnimation("Move", true);
			}
		}
		//Mira al jugador
		float angle = atan2(auxVec.x, auxVec.z);
		btQuaternion q;
		q.setX(0);
		q.setY(1 * sin(angle / 2));
		q.setZ(0);
		q.setW(cos(angle / 2));

		rb->getWorldTransform().setRotation(q);
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
}

void ButterGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
}

void ButterGuy::Spawn()
{
}

void ButterGuy::Shoot()
{
	GameObject* go = scene->Instantiate("EnemyBullet", (gameObject->getPosition() + Vector3(shootPosX, shootPosY, shootPosZ)), 0.08);
}
