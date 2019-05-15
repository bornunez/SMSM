#include "Component.h"
#include <OgreEntity.h>

class MeshRenderer : public Component
{
private:
	string meshName;
	string materialName;
	string startAnim = "";
	Ogre::Entity* entity;
	std::vector<string> materialNames;
	std::vector<Ogre::AnimationState*> animationStates;
	void Update();
	float animationVelocity = 1;

public:
	MeshRenderer(GameObject* _gameObject, string _meshName, string _materialName = "", bool _enabled = true);
	MeshRenderer(GameObject* _gameObject);

	//Animations
	void PlayAnimation(string animationName, bool loop, bool continued = false);
	void StopAnimation(bool stop);
	void InitAnimations(float velocity = 1);
	void SetAnimationSpeed(float _velocity) { animationVelocity = _velocity; };
	bool isPlaying(string name);
	bool AnimationHasEnded(string name);
	virtual void LoadFromFile(json obj);
	virtual ~MeshRenderer();

	void ResetMaterials();

	virtual void Start();
	void OnDestroy();
	void OnDisable();
	void OnEnable();
};
