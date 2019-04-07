#include "PlayerMov.h"

PlayerMov::~PlayerMov()
{
}

void PlayerMov::LoadFromFile(json obj)
{
	
}

void PlayerMov::Update()
{
	handleInput();
}

void PlayerMov::handleInput()
{
	
	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_W)) {
	//Ogre::Quaternion orientation = getGameObject()->getNode()->getOrientation();
	//orientation. Falta coger orientacion del jugador y mover en base a eso

		playerColl->getRB()->applyCentralImpulse(btVector3(0, 0, -0.5));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_S)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(0, 0, 0.5));
	}
	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_A)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(-0.5, 0, 0));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_D)) {
		playerColl->getRB()->applyCentralImpulse(btVector3(0.5, 0, 0));
	}

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
