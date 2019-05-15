#include "PlayerController.h"
#include "MyCamera.h"
#include "../../../Src/MotorEngine/GUIManager.h"
#include "../../Src/MotorEngine/MeshRenderer.h"
#include "../../Armas/Weapon.h"
#include "../../Src/MotorEngine/AudioManager.h"

PlayerController::~PlayerController()
{
}

void PlayerController::LoadFromFile(json obj)
{
	mouseSensitivity = obj["mouseSensitivity"];
	walkSpeed = obj["walkSpeed"];
	runSpeed = obj["runSpeed"];
	speed = walkSpeed;

	// Habilities ---------------------------------
	// Slow Time
	slowTimeSpeed = obj["slowTimeSpeed"];
	slowTimeDuration = obj["slowTimeDuration"];
	slowTimeCooldown = obj["slowTimeCooldown"];

	// Freeze Time
	freezeTimeSpeed = obj["freezeTimeSpeed"];
	freezeTimeDuration = obj["freezeTimeDuration"];
	freezeTimeEnemiesNeeded = obj["freezeTimeEnemiesNeeded"];

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

	pistolsGO = brazo->GetChild("PistolasDuales");
	shotgunGO = brazo->GetChild("Escopeta");
	shotgunGO->SetActive(false);

	// Crea el compositor para la sangre en pantalla al contacto
	CompositorManager::getSingleton().addCompositor(getScene()->getGame()->getViewport(), "Blood");
	CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Blood", false);

#ifndef _DEBUG
	for (int i = 0; i < lives; i++) {
		livesHeart.push_back(GUIManager::Instance()->CreateLifeIcon("livesHeart" + std::to_string(i), 0.05*(i+1), 0.05, 0.075, 0.075));
	}

	// HUD ARMAS
	pistolWindow = GUIManager::Instance()->CreateButton("null", "gunIcon", "TaharezLook/PistolaHUD", 0.025, 0.85, 0.1, 0.12, "", "null", true);
	shotGunWindow = GUIManager::Instance()->CreateButton("null", "shotGunIcon", "TaharezLook/EscopetaHUD", 0.13, 0.85, 0.1, 0.12, "", "null");
	shotGunWindow->disable();
	shotGunWindow->hide();

	// HUD TIEMPO
	slowTimeWindow = GUIManager::Instance()->CreateButton("null", "slowTimeIcon", "TaharezLook/SlowTimeHUD", 0.885, 0.155, 0.1, 0.12, "", "null", true);
	slowTimeWindow->disable();
	stopTimeWindow = GUIManager::Instance()->CreateButton("null", "stopTimeIcon", "TaharezLook/StopTimeHUD", 0.885, 0.025, 0.1, 0.12, "", "null", true);
	stopTimeWindow->disable();


	slowTimeIndicator = GUIManager::Instance()->CreateButton("null", "slowTimeInd", "TaharezLook/SlowIndHUD", 0.885, 0.155, 0.1, 0.03, "", "null",true);
	stopTimeIndicator = GUIManager::Instance()->CreateButton("null", "stopTimeInd", "TaharezLook/StopIndHUD", 0.885, 0.025, 0.1, 0.03, "", "null",true);
	CompositorManager::getSingleton().addCompositor(getScene()->getGame()->getViewport(), "Zawaru");
	CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Zawaru", false);
	CompositorManager::getSingleton().addCompositor(getScene()->getGame()->getViewport(), "Pixel");
	CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Pixel", false);

#endif

	slowTimeCooldownTimer = slowTimeCooldown;
}

void PlayerController::Update()
{
	handleInput();
	SetInvulnerability();
	habilitiesLogic();
}

void PlayerController::habilitiesLogic()
{
	if (currentHability != HabilityEnum::None)
	{
		timeElapsed += TimeManager::getInstance()->getDeltaTime();

		switch (currentHability)
		{
		case SlowTime:
			if (timeElapsed >= slowTimeDuration) {
				// Crea el compositor para la sangre en pantalla al contacto
				CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Zawaru", false);

				currentHability = HabilityEnum::None;
				gameSpeed = 1;
				slowTimeCooldownTimer = 0;				
			}
			break;
		case FreezeTime:
			if (timeElapsed >= freezeTimeDuration) {
				CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Zawaru", false);
				currentHability = HabilityEnum::None;
				gameSpeed = 1;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		slowTimeCooldownTimer += TimeManager::getInstance()->getDeltaTime();
		if (slowTimeCooldownTimer < slowTimeCooldown) {
			slowTimeIndicator->setSize(CEGUI::USize(CEGUI::UDim((slowTimeCooldownTimer/ slowTimeCooldown) * 0.1,
				0), CEGUI::UDim(0.03, 0)));
		}
	}
}

void PlayerController::handleInput()
{
	// PLAYER CAMERA ------------------------------------------------------------------------------

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

	//// PLAYER MOVEMENT --------------------------------------------------------------------------

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

	// SWITCH WEAPONS -----------------------------------------------------------------------------
	if (input->getKey(OIS::KeyCode::KC_1)) {
		switchWeapon(WeaponEnum::Pistol);
	}
	else if (input->getKey(OIS::KeyCode::KC_2)) {
		switchWeapon(WeaponEnum::ShotGun);
	}

	// PLAYER HABILITIES --------------------------------------------------------------------------

	// Slow Time
	if (currentHability == HabilityEnum::None) {
		if (input->getKey(OIS::KeyCode::KC_E) && slowTimeCooldownTimer > slowTimeCooldown) {
			CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Zawaru", true);
			currentHability = HabilityEnum::SlowTime;
			gameSpeed = slowTimeSpeed;
			timeElapsed = 0;
			// Ponemos la escala del indicador a 0
			slowTimeIndicator->setSize(CEGUI::USize(CEGUI::UDim(0, 0), CEGUI::UDim(0.12, 0)));
		}
		// Freeze Time
		else if (input->getKey(OIS::KeyCode::KC_Q) && (freezeTimeEnemyCount >= freezeTimeEnemiesNeeded)) {
			CompositorManager::getSingleton().setCompositorEnabled(getScene()->getGame()->getViewport(), "Zawaru", true);
			freezeTimeEnemyCount = 0;
			currentHability = HabilityEnum::FreezeTime;
			gameSpeed = freezeTimeSpeed;
			timeElapsed = 0;
			// Ponemos la escala del indicador a 0
			stopTimeIndicator->setSize(CEGUI::USize(CEGUI::UDim(0, 0), CEGUI::UDim(0.12, 0)));
			AudioManager::getInstance()->playSound("TimeSound", false, 1, CHANNEL::Default);
			
		}
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

void PlayerController::hideHud()
{
	for (int i = 0; i < livesHeart.size(); i++) {
		livesHeart.at(i)->hide();
	}

	pistolWindow->hide();
	shotGunWindow->hide();
	slowTimeIndicator->hide();
	stopTimeIndicator->hide();
	stopTimeWindow->hide();
	slowTimeWindow->hide();
}

Vector3  PlayerController::getPlayerDirection()
{
	return getGameObject()->getNode()->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
}

void PlayerController::switchWeapon(WeaponEnum w)
{
	currentWeapon = w;
	if (currentWeapon == WeaponEnum::Pistol) {
		// Change weapon
		pistolsGO->SetActive(true);
		shotgunGO->SetActive(false);
		// Update GUI

		shotGunWindow->disable();
		pistolWindow->enable();
	}
	else if (currentWeapon == WeaponEnum::ShotGun && shotgunUnlocked) {
		// Change weapon
		pistolsGO->SetActive(false);
		shotgunGO->SetActive(true);
		// Update GUI

		shotGunWindow->enable();
		pistolWindow->disable();
	}
}

void PlayerController::unlockWeapon(WeaponEnum w)
{
	if (w == WeaponEnum::ShotGun) {
		shotgunUnlocked = true;
		activateShotgunHUD();
	}
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
