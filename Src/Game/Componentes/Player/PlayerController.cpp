#include "PlayerController.h"
#include "MyCamera.h"

PlayerController::~PlayerController()
{
}

void PlayerController::LoadFromFile(json obj)
{
	mouseSensitivity = obj["mouseSensitivity"];
	speed = obj["speed"];

	if (obj.contains("lives")) // Se asigna una vida al player
		lives = obj["lives"];
}

void PlayerController::Start()
{
	playerColl = getComponent<PlayerCollision>();
	playerRb = playerColl->getRB();
	cam = getComponent<MyCamera>();
	input = scene->getGame()->getInputManager();

	lastMouseX = input->getMouseX();

#ifndef _DEBUG
	livesHeart = GUIManager::Instance()->getButton("livesHeart");
	livesHeart->setText(std::to_string(lives));
#endif
}

void PlayerController::Update()
{
	handleInput();
}

void PlayerController::handleInput()
{
	// PLAYER CAMERA ----------------------------------------------------------------------

	int currentMouseX = input->getMouseX();
	float xInput = input->getMouseXDif() * mouseSensitivity;

	playerRb->setAngularVelocity(btVector3(0, -xInput, 0));

	//// Reset mouse if it hits a window border
	if (currentMouseX == scene->getGame()->getRenderWindow()->getWidth() || currentMouseX == 0)
		input->CenterMouse();

	//// PLAYER MOVEMENT --------------------------------------------------------------------

	// Player Direction
	Vector3 dir = getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
	btVector3 up = btVector3(0.0, 1.0, 0.0);

	// Forward and right vector
	btVector3 forward = btVector3(dir.x, dir.y, dir.z);
	btVector3 right = forward.cross(up);

	playerRb->setLinearVelocity(btVector3(0, 0, 0));

	// Forwards / backwards
	if (input->getKey(OIS::KeyCode::KC_W)) {
		playerRb->setLinearVelocity(forward *speed);
	}
	else if (input->getKey(OIS::KeyCode::KC_S)) {
		playerRb->setLinearVelocity(-forward * speed);
	}

	// Left / Right
	if (input->getKey(OIS::KeyCode::KC_A)) {
		playerRb->setLinearVelocity(-right * speed + playerRb->getLinearVelocity());
	}
	else if (input->getKey(OIS::KeyCode::KC_D)) {
		playerRb->setLinearVelocity(right * speed + playerRb->getLinearVelocity());
	}
}

Vector3  PlayerController::getPlayerDirection()
{
	return getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
}


void PlayerController::updateLivesHeart()
{
	#ifndef _DEBUG
		livesHeart->setText(std::to_string(lives));
	#endif
}
