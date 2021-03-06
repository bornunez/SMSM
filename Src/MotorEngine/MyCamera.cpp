#include "MyCamera.h"

// Load from file del componente
void MyCamera::LoadFromFile(json obj)
{
	Initialize(obj["name"]);
}

// Inicializa la camara para los constructores
void MyCamera::Initialize(string camName)
{
	// Asigna el VP del game
	vp = getScene()->getGame()->getViewport();

	// NODOS - - - - - - - - - - - - - - - - - - - - - - - -
	name = camName;
	cam = gameObject->getSceneManager()->createCamera(camName);

	// Crea el nodo principal para la camara, que solo se encargará de la posicion
	cameraNode = gameObject->getNode();

	// Crea el nodo para el Yaw
	cameraYawNode = cameraNode->createChildSceneNode();

	// Crea el nodo para el Pitch
	cameraPitchNode = cameraYawNode->createChildSceneNode();

	// Crea el nodo para el Roll y asocia la camara a este
	cameraRollNode = cameraPitchNode->createChildSceneNode();
	cameraRollNode->attachObject(cam);

	// VALORES POR DEFECTO - - - - - - - - - - - - - - - - -
	cam->setNearClipDistance(0.01); // Dist. cercana
	cam->setFarClipDistance(10000); // Dist. lejana
	cam->setFOVy(Degree(60)); // FOV de la camara 
	cam->setAutoAspectRatio(true);

	// VIEWPORT  - - - - - - - - - - - - - - - - - - - - - -
	vp->setCamera(cam);
	
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
}

// Destructor de la clase
MyCamera::~MyCamera()
{

}

// Hacer yaw
void MyCamera::Yaw(Real degrees)
{
	cameraYawNode->yaw(Degree(degrees));
}

// Hacer pitch
void MyCamera::Pitch(Real degrees)
{
	cameraPitchNode->pitch(Degree(degrees));
}

// Hacer roll
void MyCamera::Roll(Real degrees)
{
	cameraRollNode->roll(Degree(degrees));
}

// Look At
void MyCamera::LookAt(Vector3 target, Node::TransformSpace transform_space)
{
	cameraNode->lookAt(target, transform_space);
}
