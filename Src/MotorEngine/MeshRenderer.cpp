#include "MeshRenderer.h"
#include <OgreSceneManager.h>
#include "GameObject.h"
#include "Scene.h"
#include "TimeManager.h"


MeshRenderer::MeshRenderer(GameObject * _gameObject, string _meshName, string _materialName, bool _enabled) : 
	Component(_gameObject, _enabled), meshName(_meshName), materialName(_materialName)
{
}

MeshRenderer::MeshRenderer(GameObject * _gameObject) : Component(_gameObject)
{
}

void MeshRenderer::LoadFromFile(json obj)
{
	Component::LoadFromFile(obj);
	string mesh = obj["mesh"];
	if (obj.contains("StartAnimation"))
	{
		string animAux = obj["StartAnimation"];
		startAnim = animAux;
	}
	meshName = mesh;

	if (obj.find("material") != obj.end()) {
		string mat = obj["material"];
		materialName = mat;
	}
	if (materialName != "") {
		entity->setMaterialName(materialName);
	}

	entity = gameObject->getSceneManager()->createEntity(meshName);
	
}

MeshRenderer::~MeshRenderer()
{
}  


void MeshRenderer::Start()
{
#ifdef C_DEBUG
	std::cout << "Mesh Start: " << meshName << " Nombre: " << gameObject->getName() << endl;
#endif
	if (!started) {
		gameObject->AddEntity(entity);
	}
	if (startAnim != "")
	{
		InitAnimations();
		PlayAnimation(startAnim, true);
	}
}

void MeshRenderer::OnDestroy()
{

}
void MeshRenderer::OnDisable()
{
	gameObject->RemoveEntity(entity);
}
void MeshRenderer::OnEnable()
{
	gameObject->AddEntity(entity);
}
bool MeshRenderer::AnimationHasEnded(string name)
{
	bool ended = false;
	for (int i = 0; i < animationStates.size(); i++)
	{
		if (animationStates[i]->getAnimationName() == name && animationStates[i]->getEnabled())
		{
			if (animationStates[i]->hasEnded()) ended = true;
		}
	}
	return ended;
}
void MeshRenderer::InitAnimations(float velocity)
{
	animationVelocity = velocity;
	int numAnimations = 0;
	AnimationStateSet* aux = entity->getAllAnimationStates();
	auto it = aux->getAnimationStateIterator().begin();
	while (it != aux->getAnimationStateIterator().end()) {
		auto s = it->first;
		animationStates.push_back(aux->getAnimationStates().at(s));
		animationStates[numAnimations]->setLoop(false);
		animationStates[numAnimations]->setEnabled(false);
		numAnimations++;
		++it;
	}
}

void MeshRenderer::Update()
{
	for (int i = 0; i < animationStates.size(); i++)
	{
		if (animationStates[i]->getEnabled())
		{
			animationStates[i]->addTime(TimeManager::getInstance()->getDeltaTime()*animationVelocity);
		}
	}
}
bool MeshRenderer::isPlaying(string name)
{
	bool playing = false;
	int i = 0;
	while(i < animationStates.size())
	{
		if (animationStates[i]->getAnimationName() == name && animationStates[i]->getEnabled())
		{
			playing = true;
			i = animationStates.size();
		}
		i++;
	}
	return playing;
}
void MeshRenderer::PlayAnimation(string name, bool loop, bool continued)
{
	for (int i = 0; i < animationStates.size(); i++)
	{
		if (animationStates[i]->getAnimationName() == name)
		{
			animationStates[i]->setEnabled(true);
			animationStates[i]->setLoop(loop);
			if (!continued)
				animationStates[i]->setTimePosition(0);
		}
		else animationStates[i]->setEnabled(false);
	}
}
void MeshRenderer::StopAnimation(bool stop)
{
	for (int i = 0; i < animationStates.size(); i++)
	{
		animationStates[i]->setEnabled(!stop);
	}
}
