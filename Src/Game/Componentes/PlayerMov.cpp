#include "PlayerMov.h"
#include "MyCamera.h"

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
	//mouseSensitivity = obj["mouseSensitivity"];
	//mouseSensitivity = 0.1;

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
	//// PLAYER CAMERA ----------------------------------------------------------------------

	//int currentMouseX = input->getMouseX();
	//float xInput = input->getMouseXDif() * mouseSensitivity;

	//playerRb->setAngularVelocity(btVector3(0, -xInput, 0));

	////// Reset mouse if it hits a window border
	//if (currentMouseX == scene->getGame()->getRenderWindow()->getWidth() || currentMouseX == 0)
	//	input->CenterMouse();


	////// PLAYER MOVEMENT --------------------------------------------------------------------

	//// Player Direction
	//Vector3 dir = getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
	//btVector3 up = btVector3(0.0, 1.0, 0.0);

	//// Forward and right vector
	//btVector3 forward = btVector3(dir.x, dir.y, dir.z);
	//btVector3 right = forward.cross(up);

	//playerRb->setLinearVelocity(btVector3(0, 0, 0));

	//// Forwards / backwards
	//if (input->getKey(OIS::KeyCode::KC_W)) {
	//	playerRb->setLinearVelocity(forward *speed);
	//}
	//else if (input->getKey(OIS::KeyCode::KC_S)) {
	//	playerRb->setLinearVelocity(-forward * speed);
	//}

	//// Left / Right
	//if (input->getKey(OIS::KeyCode::KC_A)) {
	//	playerRb->setLinearVelocity(-right * speed + playerRb->getLinearVelocity());
	//}
	//else if (input->getKey(OIS::KeyCode::KC_D)) {
	//	playerRb->setLinearVelocity(right * speed + playerRb->getLinearVelocity());
	//}

	//// TEST ------------------------------------------------------------------------
	//if (InputManager::getInstance()->getKey(OIS::KeyCode::KC_H)) {
	//	//cam->Yaw(5);
	//	cam->GetCameraNode()->yaw(Radian(5));
	//}
}

void PlayerMov::Start()
{
	//Obtiene el componente collider del GO para usarlo en el movimiento
	//std::list<Component*> comps = gameObject->getComponents();
	//bool found = false;
	//auto it = comps.begin();
	//while (!found && it != comps.end())
	//{
	//	PlayerCollision* c = dynamic_cast<PlayerCollision*>(*it);
	//	if (c != nullptr) {
	//		found = true;
	//		playerColl = c;
	//	}
	//	it++;
	//}

	// Components
	playerColl = getComponent<PlayerCollision>();
	playerRb = playerColl->getRB();
	cam = getComponent<MyCamera>();
	//input = scene->getGame()->getInputManager();

	//lastMouseX = InputManager::getInstance()->getMouseX();
}

void PlayerMov::Awake()
{
}