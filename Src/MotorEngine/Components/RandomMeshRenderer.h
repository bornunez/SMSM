#include "../Component.h"
#include <OgreEntity.h>

class RandomMeshRenderer : public Component
{
private:
	string meshName;
	string materialName;
	Ogre::Entity* entity;
	std::vector<Ogre::AnimationState*> animationStates;
	void Update();
	float animationVelocity = 1;

public:
	RandomMeshRenderer(GameObject* _gameObject, string _meshName, string _materialName = "", bool _enabled = true);
	RandomMeshRenderer(GameObject* _gameObject);
	//Animations
	void PlayAnimation(string animationName, bool loop, bool continued = false);
	void StopAnimation(bool stop);
	void InitAnimations(float velocity = 1);
	void AnimationSpeed(float _velocity) { animationVelocity = _velocity; };
	bool isPlaying(string name);
	bool AnimationHasEnded(string name);
	virtual void LoadFromFile(json obj);
	~RandomMeshRenderer();

	virtual void Start();
	void OnDestroy();

	void OnDisable();
	void OnEnable();
};
