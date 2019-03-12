#include "Scene.h"
#include "AudioManager.h"


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

	//Cada nodo acaba teniendo una entidad y un rigidbody asociado, que puede colocar en un grupo o en otro para poder realizar acciones sobre un conjunto
	//de rigidbodies al mismo tiempo
	
	btRigidBody* rb =  phyManager->CreateSphereCollider(testNode, 100, btVector3(0, 20, 0), btQuaternion(), 0.8f, 2);
	phyManager->AddRigidBodyToGroup(rb, 0);

	//SECOND HEAD
	Ogre::Entity* ogreEntity2 = mSceneManager->createEntity("cube.mesh");

	secondNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	secondNode->attachObject(ogreEntity2);
	secondNode->setScale(0.05, 0.05, 0.05);

	btRigidBody* rb1 = phyManager->CreateBoxCollider(secondNode, 0, btVector3(3, 0, 0), btQuaternion(), 0.5, btVector3(3,3,3));
	phyManager->AddRigidBodyToGroup(rb1, 1);

	// Crear luz

	Light* luz = mSceneManager->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);

	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));  //vec3.normalise();

	AudioManager* audioManager = new AudioManager();
	audioManager->playSound("CorazonPartio", false, 1, CHANNEL::Default);


}

// Mueve el cubo a un lado en x.
void Scene::Update() 
{
	phyManager->Update();
	//phyManager->AddDiscreteImpulse(btVector3(1, 1, 0), 20, 0);
}

Scene::~Scene()
{
}
