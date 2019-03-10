/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///-----includes_start-----
#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <OgreRoot.h>
#include "PhysicsManager.h"
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

/// This is a Hello World program for running a basic Bullet physics simulation
using namespace Ogre;

int main(int argc, char** argv)
{
	Root* root;

#if _DEBUG
	root = new Root("plugins_d.cfg");
#else
	root = new Root("plugins.cfg");
#endif

	PhysicsManager* pgr = new PhysicsManager();
	SceneManager* sceneManager = root->createSceneManager();
	sceneManager->getRootSceneNode()->createChildSceneNode();

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr planePtr = MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity *entGround = sceneManager->createEntity("GroundEntity", "ground");
	Ogre::SceneNode *groundNode = sceneManager->getRootSceneNode()->createChildSceneNode("groundNode");

	groundNode->attachObject(entGround);

	//create the plane entity to the physics engine, and attach it to the node

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));

	btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
	btVector3 localGroundInertia(0, 0, 0);

	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(groundTransform);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody *groundBody = new btRigidBody(groundRBInfo);

	//add the body to the dynamics world
	pgr->addRigidBody(groundBody);

	for (int i = 0; i < 300; i++) {
		pgr->Update();

		if (pgr != NULL) {
			pgr->getDynamicsWorld()->stepSimulation(1.0f / 60.0f); //suppose you have 60 frames per second

			for (int i = 0; i< pgr->getCollisionObjectCount(); i++) {
				btCollisionObject* obj = pgr->getDynamicsWorld()->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);

				if (body && body->getMotionState()) {
					btTransform trans;
					body->getMotionState()->getWorldTransform(trans);

					void *userPointer = body->getUserPointer();

					if (userPointer) {
						btQuaternion orientation = trans.getRotation();
						Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
						sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
						sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
					}
				}
			}
		}
	}

	delete pgr;
}

