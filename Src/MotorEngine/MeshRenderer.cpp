#include "MeshRenderer.h"
#include <OgreSceneManager.h>
#include "GameObject.h"
#include "Scene.h"


MeshRenderer::MeshRenderer(GameObject * _gameObject, string _meshName, string _materialName, bool _enabled) : 
	Component(_gameObject, _enabled), meshName(_meshName), materialName(_materialName)
{
}

MeshRenderer::~MeshRenderer()
{
}  

void MeshRenderer::Awake()
{
	entity = gameObject->getSceneManager()->createEntity(meshName);
	gameObject->AddEntity(entity);

	// Asigna el material a la mesh
	if (materialName != "") {
		entity->setMaterialName(materialName);
	}
	scene->BroadcastMessage("Hola");
}
