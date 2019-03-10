#include "Scene.h"
#include "Scene.h"


Scene::Scene(Game* _g) : g(_g)
{

	// CREACION DE ESCENA DE PRUEBA

	// Crear SceneManager
	mSceneManager = g->getRoot()->createSceneManager();

	phyManager = new PhysicsManager();

	//Crea un nodo de escena, hijo de root
	//mSceneManager->getRootSceneNode()->createChildSceneNode();

	// Crear la camara
	Camera* cam = mSceneManager->createCamera("Cam");
	cam->setNearClipDistance(6);

	mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(cam);

	mCamNode->setPosition(0, 0, 80);
	mCamNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_WORLD);

	// Crear ViewPort
	Ogre::Viewport* vp = g->getRenderWindow()->addViewport(cam);

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Escena
	Ogre::Entity* ogreEntity = mSceneManager->createEntity("ogrehead.mesh");

	testNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	testNode->attachObject(ogreEntity);
	testNode->setScale(0.1, 0.1, 0.1);

	btCollisionShape *newRigidShape = new btBoxShape(btVector3(0.8f, 0.8f, 0.8f));
	phyManager->addCollisionShape(newRigidShape);

	//set the initial position and transform. For this demo, we set the tranform to be none
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	//set the mass of the object. a mass of "0" means that it is an immovable object
	btScalar mass = 10.0f;
	btVector3 localInertia(0, 0, 0);

	startTransform.setOrigin(btVector3(0,10,0));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(1);
	body->setUserPointer(testNode);

	phyManager->getDynamicsWorld()->addRigidBody(body);


	//SECOND HEAD
	Ogre::Entity* ogreEntity2 = mSceneManager->createEntity("cube.mesh");

	secondNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	secondNode->attachObject(ogreEntity2);
	secondNode->setScale(0.05, 0.05, 0.05);

	btCollisionShape *newRigidShape2 = new btBoxShape(btVector3(5.0f, 5.0f, 5.0f));
	phyManager->addCollisionShape(newRigidShape2);

	btScalar mass2 = 0.;
	btVector3 localInertia2(0, 0, 0);


	//set the mass of the object. a mass of "0" means that it is an immovable object

	startTransform.setOrigin(btVector3(0, 0, 0));
	newRigidShape2->calculateLocalInertia(mass2, localInertia2);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *myMotionState2 = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo2(mass2, myMotionState2, newRigidShape2, localInertia2);
	btRigidBody *body2 = new btRigidBody(rbInfo2);
	body2->setRestitution(1);
	body2->setUserPointer(secondNode);

	phyManager->getDynamicsWorld()->addRigidBody(body2);
	cube = body2;

	// Crear luz

	Light* luz = mSceneManager->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);

	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));  //vec3.normalise();

}

// Mueve el cubo a un lado en x.
void Scene::Update() 
{
	//secondNode->setPosition(secondNode->getPosition().x + 0.2, secondNode->getPosition().y, testNode->getPosition().z);
	
		btTransform m;
		cube->getMotionState()->getWorldTransform(m);		
		m.setOrigin(m.getOrigin() + btVector3(0.1, 0, 0));
		cube->getMotionState()->setWorldTransform(m);
		cube->setWorldTransform(m);

		secondNode->setPosition(Ogre::Vector3(m.getOrigin().getX(), m.getOrigin().getY(), m.getOrigin().getZ()));
		//sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		
	
		phyManager->Update();
	
}

Scene::~Scene()
{
}
