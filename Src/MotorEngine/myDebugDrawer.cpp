#include "myDebugDrawer.h"
#include <iostream>

myDebugDrawer* myDebugDrawer::instance_ = nullptr;

myDebugDrawer::myDebugDrawer()
{

}

myDebugDrawer* myDebugDrawer::Instance(Ogre::SceneManager* m)
{
	if (instance_ == nullptr) {
		instance_ = new myDebugDrawer();
		instance_->sceneManager_ = m;
	}

	return instance_;
}

void myDebugDrawer::ResetInstance()
{
	delete instance_;
	instance_ = nullptr;
}

void myDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	lineCounter++;

	Ogre::Vector3 coords[2];

	coords[0].x = from.getX();
	coords[0].y = from.getY();
	coords[0].z = from.getZ();

	coords[1].x = to.getX();
	coords[1].y = to.getY();
	coords[1].z = to.getZ();

	std::string name = "LineNumber " + std::to_string(lineCounter);

	Ogre::ManualObject* line = sceneManager_->createManualObject(name);

	line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

	line->colour(Ogre::ColourValue(color.getX(), color.getY(), color.getZ()));

	line->position(coords[0]);
	line->position(coords[1]);

	line->end();

	Ogre::SceneNode* sNode = sceneManager_->getRootSceneNode()->createChildSceneNode();
	sNode->attachObject(line);
}

void myDebugDrawer::drawCube(btVector3 position, btVector3 scale)
{
	btVector3 pos = position;

	scale *= 2;

	pos.setX(pos.getX() - (scale.getX() / 2));
	pos.setY(pos.getY() - (scale.getY() / 2));
	pos.setZ(pos.getZ() - (scale.getZ() / 2));

	btVector3 baseX = pos;
	btVector3 baseY = pos;
	btVector3 baseZ = pos;
	btVector3 lastBase = pos;

	baseX.setX(pos.getX() + scale.getX());
	baseZ.setZ(pos.getZ() + scale.getZ());
	lastBase.setX(pos.getX() + scale.getX()); lastBase.setZ(pos.getZ() + scale.getZ());

	btVector3 topX = baseX;
	btVector3 topY = baseY;
	btVector3 topZ = baseZ;
	btVector3 lastTop = lastBase;

	topX.setY(topX.getY() + scale.getY());
	topY.setY(topY.getY() + scale.getY());
	topZ.setY(topZ.getY() + scale.getY());
	lastTop.setY(lastTop.getY() + scale.getY());

	btVector3 col(0, 1, 0);

	drawLine(baseY, baseX, col);
	drawLine(baseX, lastBase, col);
	drawLine(lastBase, baseZ, col);
	drawLine(baseZ, baseY, col);

	drawLine(topY, topX, col);
	drawLine(topX, lastTop, col);
	drawLine(lastTop, topZ, col);
	drawLine(topZ, topY, col);

	drawLine(topY, baseY, col);
	drawLine(topX, baseX, col);
	drawLine(lastTop, lastBase, col);
	drawLine(topZ, baseZ, col);
}

void myDebugDrawer::resetLineNumber()
{
	for (int i = 1; i < lineCounter+1; i++)
	{
		sceneManager_->destroyManualObject("LineNumber " + std::to_string(i));		
	}

	lineCounter = 0;
}
