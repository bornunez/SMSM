#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include "Movement.h"
#include "AudioManager.h"
#include "./Loaders/PrefabManager.h"

Scene::Scene(Game* _g) : g(_g)
{
	// CREACION DE ESCENA DE PRUEBA

	// Crear SceneManager
	mSceneManager = g->getRoot()->createSceneManager();

	// Crea el nodo de la escena como hijo de root
	sceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	// Crear la camara
	//cam = mSceneManager->createCamera("Cam");
	//cam->setNearClipDistance(0.01);

	//mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nCam");
	//mCamNode->attachObject(cam);

	//mCamNode->setPosition(10 * 0.5, 1, 10 * 0.5+5);
	//mCamNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_WORLD);

	// Crear ViewPort
	//Ogre::Viewport* vp = g->getRenderWindow()->addViewport(cam);

	//vp = g->getViewport();
	//vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	//cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	//AudioManager* audioManager = new AudioManager();
	//audioManager->playSound("CorazonPartio", false, 1, CHANNEL::Default);

	myDebugDrawer::Instance(mSceneManager);
}

Scene::~Scene() {}

void Scene::Release()
{
	//ToDo: release memory used by the scene
}

void Scene::parroThings(SceneManager* mSceneManager)
{
	//BICHO
	//PrefabManager::getInstance()->Instantiate("Escopeta", this, nullptr, {-3, -5, 35}, 1);
	/*Instantiate("Pistola", { (Ogre::Real)4.85, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);
	Instantiate("Pistola2", { (Ogre::Real)5.15, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);*/
}

void Scene::Load(json sceneFile)
{

	//CARGA DE ESCENA POR FICHEROS / PONER LOS OBJETOS A MANO

	LoadFromFile(sceneFile);

	parroThings(mSceneManager);
	//PrefabManager::getInstance()->Instantiate("Cube", this, nullptr, { 0,0,0 }, 0.1);

	for (Component* c : components) {
		if (!c->isAwake()) {
			c->Awake();
			c->setAwake();
		}
	}
}

void Scene::LoadFromFile(json sceneFile)
{
	//Cargar GameObjects
	cout << "\n\n==================================================\n";
	cout << "============    CARGA DE ESCENA       ============\n";
	cout << "==================================================\n\n";
	cout << "Cargando escena " << sceneFile["sceneName"] << endl << endl;
	if (!sceneFile["GameObjects"].empty()) {
		cout << "Existen GameObjects a cargar" << endl << endl;
		for (auto &pref : sceneFile["GameObjects"])
			if (pref.is_object()) {
				//GameObject* o = ResourcesManager::GetInstance()->GetPrefabManager()->GenerateGameObject(pref["prefabName"], this, nullptr);
				GameObject* o = PrefabManager::getInstance()->ParseGameObject(pref, this, nullptr);

				//Si existe un prefab con el nombre, lo rellenamos
				if (o != nullptr) {
					Add(o);
					cout << "Loaded prefab: " << pref["prefabName"] << " succesfully" << endl << endl;
				}
			}
	}

	if (sceneFile.contains("Skybox")) {
		json skyObj = sceneFile["Skybox"];
		mSceneManager->setSkyDome(true, "SMSM/Skybox");
	}
	cout << "==================================================\n\n";
}

void Scene::SetActive(bool active)
{
	//vp->setCamera(cam);
	// ToDo: activar o desactivar componentes
}

void Scene::Start()
{
	started = true;
	for (Component* c : components) {
		if (c->isActiveAndEnabled() && !c->isStarted()) {
			c->Start();
			c->setStarted();
		}
	}
}

// Mueve el cubo a un lado en x.
void Scene::Update() 
{
	PhysicsManager::Instance()->Update();

	//testNode->setPosition(testNode->getPosition().x + 1,testNode->getPosition().y, testNode->getPosition().z);

	//Recorremos los updates de los objetos activos
	//cout << "N� Componentes: " << components.size() << endl;
	for (Component* c : components) {
		/*cout << "Active: " << c->getGameObject()->isActive() << endl;
		cout << "Enabled: " << c->Enabled() << endl;*/
		if (c->isActiveAndEnabled()) c->Update();
	}
	//Tambien el late update de los objs activos
	for (Component* c : components) {
		if (c->isActiveAndEnabled()) c->LateUpdate();
	}

	//phyManager->LateUpdate();

	//Finalmente limpiamos todos los objetos pendientes de borrar
	ClearTrash();
}

void Scene::Add(GameObject * o)
{
	//A�adimos el obeto a la escena
	gameObjects.push_back(o);
	////Y tambien sus componentes
	//for (Component* c : o->getComponents())
	//	components.push_back(c);
}

void Scene::Add(Component * c)
{
	components.push_back(c);
}

GameObject* Scene::Instantiate(GameObject * o, Vector3 position, float scale, GameObject * parent)
{
	if (o != nullptr) {
		if (parent != nullptr) {
			parent->AddChild(o);
		}
		o->setPosition(position);
		o->setScale(scale);
		Add(o);

		//Llamamos al awake del objeto, independientemente de si esta activo o no
		for (Component* c : o->getComponents()) {
			if (!c->isAwake()) {
				c->Awake();
				c->setAwake();
			}
		}
		//Llamamos al start solo de los objetos y componentes que esten activos
		for (Component* c : o->getComponents()) {
			if (c->isActiveAndEnabled() && !c->isStarted()) {
				c->Start();
				c->setAwake();
			}
		}
	}
}

GameObject* Scene::Instantiate(string prefab, Vector3 position, float scale, GameObject * parent)
{
	GameObject* o = PrefabManager::getInstance()->Instantiate(prefab, this, parent, position, scale);
	o = Instantiate(o, position, scale, parent);
	return o;
}

void Scene::Destroy(GameObject * o)
{
	trash.push(o);
}

void Scene::BroadcastMessage(string message)
{
	for (GameObject* o : listeners) {
		if (o->isActive())
			o->BroadcastMessage(message);
	}


}

GameObject * Scene::getGameObject(string name)
{
	bool found = false;
	auto it = gameObjects.begin();
	while (!found && it != gameObjects.end())
		found = (*it)->getName() == name;
	return found ? *it : nullptr;
}

void Scene::ClearTrash()
{
	//Vamos a vaciar toda la basura generada en el ciclo principal
	while (!trash.empty()) {
		GameObject* o = trash.front(); trash.pop();

		//Primero le vaciamos los componentes al objeto. 
		for (Component* c : o->getComponents()) {
			c->OnDestroy();
			o->RemoveComponent(c);
			//Luego los quitamos de la escena
			//Y finalmente lo borramos
			components.remove(c);
			delete	c;
		}
		//Una vez vaciado el objeto, lo quitamos de la escena y finalmente lo borramos
		mSceneManager->destroySceneNode(o->getNode());
		gameObjects.remove(o);
		delete o;
	}
}
