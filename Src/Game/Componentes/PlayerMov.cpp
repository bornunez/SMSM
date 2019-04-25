#include "PlayerMov.h"

PlayerMov::~PlayerMov()
{
}

Vector3 PlayerMov::getPlayerDirection()
{
	return getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
}

void PlayerMov::LoadFromFile(json obj)
{
	speed = obj["speed"];
	/*maxRotSpeed = obj["maxRotSpeed"];
	movSpeed = obj["movSpeed"];
	maxSpeed = obj["maxSpeed"];*/
}

void PlayerMov::Update()
{
	handleInput();
}

void PlayerMov::handleInput()
{
	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_LEFT) && playerRb->getAngularVelocity().y() < 10/*maxRotSpeed*/) {
		playerRb->applyTorqueImpulse(btVector3(0, 0.05, 0));
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_RIGHT) && playerRb->getAngularVelocity().y() > -10/*maxRotSpeed*/) {
		playerRb->applyTorqueImpulse(btVector3(0, -0.05, 0));
	}
	else if(!InputManager::getInstance()->getKey(OIS::KeyCode::KC_RIGHT) && !InputManager::getInstance()->getKey(OIS::KeyCode::KC_LEFT)){
		playerRb->setAngularVelocity(btVector3(0, 0, 0));
	}

	// Player Direction
	Vector3 dir = getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
	btVector3 up = btVector3(0.0, 1.0, 0.0);

	// Forward and right vector
	btVector3 forward = btVector3(dir.x, dir.y, dir.z);
	btVector3 right = forward.cross(up);

	playerRb->setLinearVelocity(btVector3(0, 0, 0));

	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_W)) {
		playerRb->setLinearVelocity(forward *speed);
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_S)) {
		playerRb->setLinearVelocity(-forward * speed);
	}

	if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_A)) {
		playerRb->setLinearVelocity(-right * speed + playerRb->getLinearVelocity());
	}
	else if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_D)) {
		playerRb->setLinearVelocity(right * speed + playerRb->getLinearVelocity());
	}
}

void PlayerMov::Start()
{
	playerRb = playerColl->getRB();
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

	playerRb = playerColl->getRB();
}