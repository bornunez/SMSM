#include "Ogre.h"
#include <btBulletCollisionCommon.h>

class myMotionState : public btMotionState
{
protected:
	Ogre::SceneNode* _visibleobj;
	btTransform _pos;

public:
	myMotionState(const btTransform &initialPos, Ogre::SceneNode* node);
	virtual ~myMotionState();

	void setNode(Ogre::SceneNode* node) { _visibleobj = node; }
	virtual void getWorldTransform(btTransform &worldTr) const;
	virtual void setWorldTransform(const btTransform &worldTr);
};

