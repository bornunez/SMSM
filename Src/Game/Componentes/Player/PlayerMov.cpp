#include "PlayerMov.h"
#include "../../../Src/MotorEngine/InputManager.h"
#include "../../../Src/MotorEngine/Scene.h"
#include "../../../Src/MotorEngine/AudioManager.h"
#include "../../../Src/MotorEngine/Loaders/PrefabManager.h"

PlayerMov::~PlayerMov()
{
}

void PlayerMov::LoadFromFile(json obj)
{
}

void PlayerMov::Update()
{
}

void PlayerMov::handleInput()
{
	if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_W)) {
		
		//getGameObject()->getNode().
	}



}

void PlayerMov::Start()
{
}
