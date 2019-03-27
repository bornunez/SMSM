#include "myMotionState.h"

myMotionState::myMotionState(const btTransform & initialPos, Ogre::SceneNode * node)
{
	_visibleobj = node;
	_pos = initialPos;
}

myMotionState::~myMotionState()
{
}

void myMotionState::getWorldTransform(btTransform & worldTr) const
{
	worldTr = _pos;
}

// Método para modificar el transform de los objetos fisicos desde el mundo visible
void myMotionState::setWorldTransform(const btTransform & worldTr)
{
	if (NULL == _visibleobj) return;
	btQuaternion rot = worldTr.getRotation();
	_visibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	btVector3 pos = worldTr.getOrigin();
	_visibleobj->setPosition(pos.x(), pos.y(), pos.z());
}
