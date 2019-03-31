#include "RigidBodyComponent.h"

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::Update()
{
	resetHitState();
}

void RigidBodyComponent::resetHitState()
{
	if (hit && timeSinceLastHit > recoilTime)
	{
		hit = false;
		timeSinceLastHit = 0;
	}
	else
	{
		timeSinceLastHit += TimeManager::getInstance()->getDeltaTime();
	}
}
