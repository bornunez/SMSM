#include "MyCamera.h"

// Constructor poniendo el viewport en una ventana
MyCamera::MyCamera(string camName, SceneNode* rootNode, SceneManager* sceneManager, RenderWindow* renderWindow) : name(camName), sm(sceneManager), rw(renderWindow)
{
	Initialize(camName, rootNode, sceneManager);

	vp = renderWindow->addViewport(cam);
}

// Constructor poniendo el viewport en una textura
MyCamera::MyCamera(string camName, SceneNode* rootNode, SceneManager* sceneManager, RenderTexture* renderTexture) : name(camName), sm(sceneManager), rt(renderTexture)
{
	Initialize(camName, rootNode, sceneManager);

	vp = renderTexture->addViewport(cam);
}

// Inicializa la camara para los constructores
void MyCamera::Initialize(string camName, SceneNode* rootNode, SceneManager* sceneManager)
{
	cam = sceneManager->createCamera(camName);

	// Crea el nodo principal para la camara, que solo se encargará de la posicion
	cameraNode = rootNode->createChildSceneNode();

	// Crea el nodo para el Yaw
	cameraYawNode = cameraNode->createChildSceneNode();

	// Crea el nodo para el Pitch
	cameraPitchNode = cameraYawNode->createChildSceneNode();

	// Crea el nodo para el Roll y asocia la camara a este
	cameraRollNode = cameraPitchNode->createChildSceneNode();
	cameraRollNode->attachObject(cam);

	// VALORES POR DEFECTO - - - - - - - - - - - - - - - - -
	cam->setNearClipDistance(0.1); // Dist. cercana
	cam->setFarClipDistance(10000); // Dist. lejana
	cam->setFOVy(Degree(60)); // FOV de la camara 
	cam->setAutoAspectRatio(true);
}

// Destructor de la clase, destruye los nodos y la camara
MyCamera::~MyCamera()
{
	sm->destroyEntity(name);
	sm->destroySceneNode(cameraRollNode);
	sm->destroySceneNode(cameraPitchNode);
	sm->destroySceneNode(cameraYawNode);
	sm->destroySceneNode(cameraNode);

	if (rw != nullptr)
		rw->removeAllViewports();

	if (rt != nullptr)
		rt->removeAllViewports();
}

// Metodo para cambiar la posicion de la camara
void MyCamera::SetPosition(Real x, Real y, Real z)
{
	cameraNode->setPosition(x, y, z);
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
