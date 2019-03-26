#pragma once
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderTexture.h>
#include <OgreRenderWindow.h>

using namespace std;
using namespace Ogre;

class MyCamera
{
public:
	MyCamera() {};
	MyCamera(string camName, SceneNode *rootNode, SceneManager* sceneManager, RenderWindow* renderWindow);
	MyCamera(string camName, SceneNode *rootNode, SceneManager* sceneManager, RenderTexture* renderTexture);
	~MyCamera();

	// Devuelve la camara de ogre para poder realizar operaciones mas concretas
	Camera* GetCamera() { return cam; };

	// Devuelve los distintos nodos de la camara
	SceneNode* GetCameraNode() { return cameraNode; };
	SceneNode* GetCameraYawNode() { return cameraYawNode; };
	SceneNode* GetCameraPitchNode() { return cameraPitchNode; };
	SceneNode* GetCameraRollNode() { return cameraRollNode; };

	// Translaciones y rotaciones
	void SetPosition(Real x, Real y, Real z);
	void Yaw(Real degrees);
	void Pitch(Real degrees);
	void Roll(Real degrees);

	// FOV
	Degree GetFOV() { cam->getFOVy(); };
	void SetFOV(Real degrees) { cam->setFOVy(Degree(degrees)); };

	// Aspect Ratio
	Real GetAspectRatio() { cam->getAspectRatio(); };
	void SetAspectRatio(Real ratio) { cam->setAspectRatio(ratio); };

	// Viewport
	Viewport* GetViewport() { return vp; };

private:
	// Camara de ogre
	Camera* cam;
	string name;

	// Nodos para el correcto funcionamiento de la camara
	SceneNode *cameraNode;
	SceneNode *cameraYawNode;
	SceneNode *cameraPitchNode;
	SceneNode *cameraRollNode;

	// Scene Manager
	SceneManager* sm;

	// Viewport
	Viewport* vp = nullptr;
	RenderWindow* rw = nullptr;
	RenderTexture* rt = nullptr;

	// Metodo inicializador
	void Initialize(string camName, SceneNode *rootNode, SceneManager* sceneManager);
};

