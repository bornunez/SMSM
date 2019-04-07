#include "PlayerMov.h"

PlayerMov::~PlayerMov()
{
}

void PlayerMov::LoadFromFile(json obj)
{
	//maxRotSpeed = obj["maxRotSpeed"];
	//movSpeed = obj["movSpeed"];
}

void PlayerMov::Update()
{
	handleInput();
}

void PlayerMov::handleInput()
{
	

	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_LEFT) && playerColl->getRB()->getAngularVelocity().y() < 10/*maxRotSpeed*/) {
		playerColl->getRB()->applyTorqueImpulse(btVector3(0, 0.05, 0));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_RIGHT) && playerColl->getRB()->getAngularVelocity().y() > -10/*maxRotSpeed*/) {
		playerColl->getRB()->applyTorqueImpulse(btVector3(0, -0.05, 0));
	}
	else if(!InputManager::getInstance()->getKey(OIS::KeyCode::KC_RIGHT) && !InputManager::getInstance()->getKey(OIS::KeyCode::KC_LEFT)){
		playerColl->getRB()->setAngularVelocity(btVector3(0, 0, 0));
	}

	
	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_W)) {
	//Ogre::Quaternion orientation = getGameObject()->getNode()->getOrientation();
	//orientation. Falta coger orientacion del jugador y mover en base a eso

		playerColl->getRB()->applyCentralImpulse(btVector3(0, 0, -0.5/*movSpeed*/));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_S)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(0, 0, 0.5/*movSpeed*/));
	}
	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_A)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(-0.5/*movSpeed*/, 0, 0));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_D)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(0.5/*movSpeed*/, 0, 0));
	}
;
	// Para el movimiento de forma brusca si no se pulsa una tecla, temporal, necesitamos un anykey
	if(!InputManager::getInstance()->getKey(OIS::KeyCode::KC_W) &&
		!InputManager::getInstance()->getKey(OIS::KeyCode::KC_A) &&
		!InputManager::getInstance()->getKey(OIS::KeyCode::KC_D) &&
		!InputManager::getInstance()->getKey(OIS::KeyCode::KC_S))
			playerColl->getRB()->setLinearVelocity(btVector3(0, 0, 0));
}

void PlayerMov::Start()
{
	
}

void PlayerMov::Awake()
{
	//Obtiene el componente collider del GO para usarlo en el movimiento
	std::list<Component*> comps = gameObject->getComponents();
	bool found = false;
	auto it = comps.begin();
	while (!found && it != comps.end())
	{
		PlayerCollision* c = dynamic_cast<PlayerCollision*>(*it);
		if (c != nullptr) {
			found = true;
			playerColl = c;
		}
		it++;
	}
}
