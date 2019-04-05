#pragma once
#include "Component.h"
#include "Scene.h"
#include "GameObject.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderTexture.h>
#include <OgreRenderWindow.h>
#include <string>

using namespace std;
using namespace Ogre;

class MyCamera : public Component
{
public:
	MyCamera(GameObject* obj) : Component(obj) {};
	~MyCamera();

	// Devuelve la camara de ogre para poder realizar operaciones mas concretas
	Camera* GetCamera() { return cam; };

	// Devuelve los distintos nodos de la camara
	SceneNode* GetCameraNode() { return cameraNode; };
	SceneNode* GetCameraYawNode() { return cameraYawNode; };
	SceneNode* GetCameraPitchNode() { return cameraPitchNode; };
	SceneNode* GetCameraRollNode() { return cameraRollNode; };

	// Translaciones y rotaciones
	void Yaw(Real degrees);
	void Pitch(Real degrees);
	void Roll(Real degrees);
	void LookAt(Vector3 target, Node::TransformSpace transform_space = Ogre::Node::TS_WORLD);

	// FOV
	Degree GetFOV() { return cam->getFOVy(); };
	void SetFOV(Real degrees) { cam->setFOVy(Degree(degrees)); };

	// Aspect Ratio
	Real GetAspectRatio() { return cam->getAspectRatio(); };
	void SetAspectRatio(Real ratio) { cam->setAspectRatio(ratio); };

	// Viewport
	Viewport* GetViewport() { return vp; };
	void SetVPBackgroundColour(Ogre::ColourValue colour) { vp->setBackgroundColour(colour); };

	// Component method
	void LoadFromFile(json obj);

private:
	// Camara de ogre
	Camera* cam;
	std::string name;

	// Nodos para el correcto funcionamiento de la camara
	SceneNode *cameraNode;
	SceneNode *cameraYawNode;
	SceneNode *cameraPitchNode;
	SceneNode *cameraRollNode;

	// Scene Manager
	SceneManager* sm;

	// Viewport
	Viewport* vp = nullptr;

	// Metodo inicializador
	void Initialize(string camName);
};

