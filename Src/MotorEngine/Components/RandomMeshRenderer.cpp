#include "RandomMeshRenderer.h"
#include <OgreSceneManager.h>
#include "../GameObject.h"
#include "../Scene.h"
#include "../TimeManager.h"


RandomMeshRenderer::RandomMeshRenderer(GameObject * _gameObject, string _meshName, string _materialName, bool _enabled) : 
	Component(_gameObject, _enabled), meshName(_meshName), materialName(_materialName)
{
}

RandomMeshRenderer::RandomMeshRenderer(GameObject * _gameObject) : Component(_gameObject)
{
}

void RandomMeshRenderer::LoadFromFile(json obj)
{
	Component::LoadFromFile(obj);

	json meshObj = obj["mesh"];
	int meshIdx = rand() % meshObj.size();
	cout << "Elegida la mesh n: " << meshIdx << " de " << meshObj.size() << endl;
	string mesh = meshObj.at(meshIdx);
	meshName = mesh;

	if (obj.find("material") != obj.end()) {
		string mat = obj["material"];
		materialName = mat;
	}
	if (materialName != "") {
		entity->setMaterialName(materialName);
	}

	entity = gameObject->getSceneManager()->createEntity(meshName);
	
	// Asigna el material a la mesh
}

RandomMeshRenderer::~RandomMeshRenderer()
{
}  


void RandomMeshRenderer::Start()
{
	std::cout << "Mesh Start: " << meshName << " Nombre: " << gameObject->getName() << endl;
	if (!started) {
		gameObject->AddEntity(entity);
	}
}

void RandomMeshRenderer::OnDestroy()
{
	//gameObject->getNode()->detachObject()
}
void RandomMeshRenderer::OnDisable()
{
	gameObject->RemoveEntity(entity);
}
void RandomMeshRenderer::OnEnable()
{
	gameObject->AddEntity(entity);
}
bool RandomMeshRenderer::AnimationHasEnded(string name)
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
void RandomMeshRenderer::InitAnimations(float velocity)
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

void RandomMeshRenderer::Update()
{
	for (int i = 0; i < animationStates.size(); i++)
	{
		if (animationStates[i]->getEnabled())
		{
			animationStates[i]->addTime(TimeManager::getInstance()->getDeltaTime()*animationVelocity);
		}
	}
}
bool RandomMeshRenderer::isPlaying(string name)
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
void RandomMeshRenderer::PlayAnimation(string name, bool loop, bool continued)
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
void RandomMeshRenderer::StopAnimation(bool stop)
{
	for (int i = 0; i < animationStates.size(); i++)
	{
		animationStates[i]->setEnabled(!stop);
	}
}
