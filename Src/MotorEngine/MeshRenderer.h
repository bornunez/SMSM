#include "Component.h"
#include <OgreEntity.h>

class MeshRenderer : public Component
{
private:
	string meshName;
	string materialName;
	Ogre::Entity* entity;

public:
	MeshRenderer(GameObject* _gameObject, string _meshName, string _materialName = "", bool _enabled = true);
	~MeshRenderer();

	virtual void Awake();
};
