#include "IncognitoGuy.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

IncognitoGuy::~IncognitoGuy()
{
}

void IncognitoGuy::Start() {
	Enemy::Start();

	meshRend = gameObject->getComponent<MeshRenderer>();

	meshRend->InitAnimations();

	meshRend->PlayAnimation("Move", true);
	meshRend->AnimationSpeed(2);

	gameObject->setScale(scale);
	
	tm = TimeManager::getInstance();

	rng = default_random_engine{};

	xVec.resize(posVec.size() * 2);
	zVec.resize(posVec.size() * 2);
	for (int i = 0; i<posVec.size(); i++) {
		xVec.push_back(posVec[i]);
		zVec.push_back(posVec[i]);
		xVec.push_back(-(float)posVec[i]);
		zVec.push_back(-(float)posVec[i]);
	}
	
	RandomizeVecs();
}
void IncognitoGuy::LoadFromFile(json obj)
{
	//Params from file
	//rb->setDamping(obj["linDamp"], obj["angDamp"]);
	scale = obj["scale"];
	gravity = obj["gravity"];
	moveSpeed = obj["moveSpeed"];
	tpTime = obj["tpTime"];
	posVec = obj["posVec"];

	Enemy::alive = true;
}


void IncognitoGuy::Update()
{
	if (estado != state::DEAD) {
		if (estado == state::IDLE) {
			tpTimer += tm->getDeltaTime();
			if (tpTimer >= tpTime) {
				//Teletransportar

				//Calculo orientacion
				Ogre::Vector3 dir = player->getPosition() - gameObject->getPosition();
				float angle = atan2(dir.x, dir.z);
				btQuaternion q;
				q.setX(0);
				q.setY(1 * sin(angle / 2));
				q.setZ(0);
				q.setW(cos(angle / 2));
				//Asignar orientacion
				rb->getWorldTransform().setRotation(q);
				rb->clearForces();

				Teleport();
				tpTimer = 0;
			}			
		}
	}
	// Si esta muerto y su animacion de muerte ha terminado...
	else if (meshRend->AnimationHasEnded("Death")) {
		Enemy::OnDeath();
	}
}

void IncognitoGuy::OnDeath() {
	estado = state::DEAD;
	rb->clearForces();
	meshRend->PlayAnimation("Death", false);
	meshRend->AnimationSpeed(2);
}

void IncognitoGuy::Spawn()
{
}

void IncognitoGuy::Teleport()
{
	// Obtenemos transform actual
	btTransform curT;
	rb->getMotionState()->getWorldTransform(curT);

	// Sumamos desplazamiento y aplicamos transform
	btVector3 newOrigin = btVector3{xVec[posIndex], 0, zVec[posIndex]};
	newOrigin += curT.getOrigin();
	curT.setOrigin(newOrigin);
	rb->setWorldTransform(curT);

	cout << "Pos X: " << newOrigin.x() << " Pos Y: " << newOrigin.z() << endl;

	btTransform testT;
	rb->getMotionState()->getWorldTransform(testT);
	btVector3 testOrigin = testT.getOrigin();

	cout << "New X: " << testOrigin.x() << " New Y: " << testOrigin.z() << endl;

	// Generar particulas
	//scene->Instantiate("PoofPS", gameObject->getPosition(), 0.025f);
	posIndex++;

	if (posIndex > (xVec.size())) {
		RandomizeVecs();
		posIndex = 0;
	}

	rb->clearForces();
}

void IncognitoGuy::RandomizeVecs()
{
	shuffle(begin(xVec), end(xVec), rng);
	shuffle(begin(zVec), end(zVec), rng);
}
