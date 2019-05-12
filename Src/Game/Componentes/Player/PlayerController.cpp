#include "PlayerController.h"
#include "MyCamera.h"
#include "../../../Src/MotorEngine/GUIManager.h"
#include "../../Src/MotorEngine/MeshRenderer.h"

PlayerController::~PlayerController()
{
}

void PlayerController::LoadFromFile(json obj)
{
	mouseSensitivity = obj["mouseSensitivity"];
	walkSpeed = obj["walkSpeed"];
	runSpeed = obj["runSpeed"];
	speed = walkSpeed;

	// Tiempo invulnerable
	recoverTime = obj["recoverTime"];

	if (obj.contains("lives")) { // Se asigna una vida al player
		lives = obj["lives"];
		maxHealth = lives;
	}
}

void PlayerController::Start()
{
	playerColl = gameObject->getComponent<PlayerCollision>();
	playerRb = playerColl->getRB();
	cam = gameObject->getComponent<MyCamera>();
	input = scene->getGame()->getInputManager();

	lastMouseX = input->getMouseX();

	brazo = gameObject->GetChild("Brazo");
	if (brazo == nullptr) {
#ifdef C_DEBUG
		cout << "ERROR: No se ha encontrado el brazo del player" << endl;
#endif
	}

	// Crea el compositor para la sangre en pantalla al contacto
	CompositorManager::getSingleton().addCompositor(getScene()->getGame()->getViewport(), "Blood");
	CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Blood", false);

#ifndef _DEBUG
	for (int i = 0; i < lives; i++) {
		livesHeart.push_back(GUIManager::Instance()->CreateLifeIcon("livesHeart" + std::to_string(i), 0.05*(i+1), 0.05, 0.075, 0.075));
	}
#endif
}

void PlayerController::Update()
{
	handleInput();
	SetInvulnerability();
}

void PlayerController::handleInput()
{
	// PLAYER CAMERA ----------------------------------------------------------------------

	int currentMouseX = input->getMouseX();
	int currentMouseY = input->getMouseY();
	float xInput = input->getMouseXDif() * mouseSensitivity * 1.57;
	float yInput = input->getMouseYDif() * mouseSensitivity * 0.0225;
	yAngle += yInput;
	if (yAngle > 0.7)
		yAngle = 0.7;
	else if (yAngle < -0.7)
		yAngle = -0.7;

	//// Reset mouse if it hits a window border
	if (currentMouseX == scene->getGame()->getRenderWindow()->getWidth() || currentMouseX == 0)
		input->CenterMouse();

	if (currentMouseY == scene->getGame()->getRenderWindow()->getHeight() || currentMouseY == 0)
		input->CenterMouse();

	//// PLAYER MOVEMENT --------------------------------------------------------------------

	// Running
	if (input->getKey(OIS::KeyCode::KC_LSHIFT)) {
		speed = runSpeed;
	}
	else {
		speed = walkSpeed;
	}

	// Player Direction
	Vector3 dir = getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
	btVector3 up = btVector3(0.0, 1.0, 0.0);

	// Forward and right vector
	btVector3 forward = btVector3(dir.x, dir.y, dir.z);
	btVector3 right = forward.cross(up);


	forward.setY(0);
	right.setY(0);
	
	playerRb->setAngularVelocity(up * -xInput);
	Quaternion q = brazo->getNode()->getOrientation();
	q.FromAngleAxis(Ogre::Radian::Radian(-yAngle),Vector3(1,0,0));
	brazo->getNode()->setOrientation(q);

	playerRb->setLinearVelocity(btVector3(0, 0, 0));

	playerRb->activate();

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

	if (input->getKey(OIS::KeyCode::KC_L)) {
		cout << "SlowMotion: ON" << endl;
		gameSpeed = 0.2f;
	}
	else if (input->getKey(OIS::KeyCode::KC_K)) {
		cout << "SlowMotion: OFF" << endl;
		gameSpeed = 1;
	}
}

void PlayerController::modifySensitivity(bool v)
{
	if (v) {
		mouseSensitivity += 0.05f;
		
		if (mouseSensitivity > 0.5f) 
			mouseSensitivity = 0.5f;		
		else 
			sensitivityLevel++;
	}
	else {
		mouseSensitivity -= 0.05f;

		if (mouseSensitivity < 0.05f)
			mouseSensitivity = 0.05f;
		else
			sensitivityLevel--;
	}
#ifndef _DEBUG
	GUIManager::Instance()->getButton("Sensitivity")->setText("Sensitivity: " + std::to_string(sensitivityLevel));
#endif
}

void PlayerController::receiveDamage()
{
	if (!invulnerability && lives > 0) {
		
		CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Blood", true);

		invulnerability = true;
		lives--;
#ifndef _DEBUG
		livesHeart.at(lives)->hide();
#endif
		if (lives == 0) {
#ifdef C_DEBUG
			cout << endl << "MUERTE" << endl << endl;
#endif
			GUIManager::Instance()->GameOver();
		}
	}
}

void PlayerController::gainHealth()
{
	if (lives < maxHealth) {
#ifndef _DEBUG
		livesHeart.at(lives)->show();
#endif		
		lives++;
	}
}

void PlayerController::hideHealth()
{
	for (int i = 0; i < livesHeart.size(); i++) {
		livesHeart.at(i)->hide();
	}
}

Vector3  PlayerController::getPlayerDirection()
{
	return getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
}

void PlayerController::SetInvulnerability() {

	if (invulnerability  && actRecoverTime < recoverTime) {
		actRecoverTime += TimeManager::getInstance()->getDeltaTime();
		// Cuando se llega  a la mitad del tiempo de invulnerable se quita la sangre en pantalla
		if (actRecoverTime > (recoverTime / 2)) {
			CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Blood", false);
		}
		
	}
	else {
		invulnerability = false;
		actRecoverTime = 0;
	}
}