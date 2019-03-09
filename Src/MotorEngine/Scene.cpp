#include "Scene.h"
#include "Scene.h"


Scene::Scene(Game* _g) : g(_g)
{

	// CREACION DE ESCENA DE PRUEBA

	// Crear SceneManager
	mSceneManager = g->getRoot()->createSceneManager();

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
	Ogre::Entity* ogreEntity = mSceneManager->createEntity("cube.mesh");

	testNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	testNode->attachObject(ogreEntity);
	testNode->setScale(0.1, 0.1, 0.1);


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
	testNode->setPosition(testNode->getPosition().x + 0.1, testNode->getPosition().y, testNode->getPosition().z);
}

Scene::~Scene()
{
}
