#include "Component.h"
#include <OgreEntity.h>

class MeshRenderer : public Component
{
private:
	string meshName;
	string materialName;
	Ogre::Entity* entity;
	std::vector<Ogre::AnimationState*> animationStates;
	void Update();
	float animationVelocity = 1;

public:
	MeshRenderer(GameObject* _gameObject, string _meshName, string _materialName = "", bool _enabled = true);
	MeshRenderer(GameObject* _gameObject);
	void PlayAnimation(string animationName, bool loop, bool continued = false);
	void StopAnimation(bool stop);
	void InitAnimations(float velocity = 1);
	void AnimationSpeed(float _velocity) { animationVelocity = _velocity; };
	bool AnimationHasEnded(string name);
	virtual void LoadFromFile(json obj);
	~MeshRenderer();

	virtual void Awake();
};
