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

	myDebugDrawer::Instance(mSceneManager);
}

Scene::~Scene() {
	Release();
}

void Scene::Release()
{
	destroying = true;

#ifdef C_DEBUG
	cout << endl << endl << "=======================================================================" << endl << endl;
	cout << endl << "BORRANDO ESCENA " << endl << endl;
	//Eliminar todos los GameObjects y sus componentes
	cout << "Borrando GameObjects" << endl;
#endif
	for (GameObject* go : gameObjects) 
		//delete go;
		Destroy(go);

	ClearTrash();
#ifdef C_DEBUG
	cout << "GameObjects borrados con exito" << endl;
	
	cout << "Borrando RigidBodies" << endl;
#endif

	// Borra el mundo actual
	PhysicsManager::Instance()->resetWorld();

	// Crea un nuevo PhysicsManager
	PhysicsManager::Instance();

#ifdef C_DEBUG
	cout << "RigidBodies borrados con exito" << endl;

	cout << "Escena borrada con exito" << endl;
#endif
	destroying = false;
}

//void Scene::parroThings(SceneManager* mSceneManager)
//{
//	//BICHO
//	//PrefabManager::getInstance()->Instantiate("Escopeta", this, nullptr, { (Ogre::Real)5.1, (Ogre::Real)0.875, (Ogre::Real)4.6 + 4.5 }, 0.02);
//	//PrefabManager::getInstance()->Instantiate("Pistola", this, nullptr, { (Ogre::Real)4.85, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);
//	//PrefabManager::getInstance()->Instantiate("Pistola2", this, nullptr, { (Ogre::Real)5.15, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);
//	//PrefabManager::getInstance()->Instantiate("Escopeta", this, nullptr, {-3, -5, 35}, 1);
//	/*Instantiate("Pistola", { (Ogre::Real)4.85, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);
//	Instantiate("Pistola2", { (Ogre::Real)5.15, (Ogre::Real)0.8, (Ogre::Real)4.6+5 }, 0.05);*/
//}

void Scene::Load(json sceneFile)
{

	//CARGA DE ESCENA POR FICHEROS / PONER LOS OBJETOS A MANO

	LoadFromFile(sceneFile);

	//parroThings(mSceneManager);
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
#ifdef C_DEBUG
	cout << "\n\n==================================================\n";
	cout << "============    CARGA DE ESCENA       ============\n";
	cout << "==================================================\n\n";
	cout << "Cargando escena " << sceneFile["sceneName"] << endl << endl;
#endif
	if (!sceneFile["GameObjects"].empty()) {
#ifdef C_DEBUG
		cout << "Existen GameObjects a cargar" << endl << endl;
#endif
		for (auto &pref : sceneFile["GameObjects"])
			if (pref.is_object()) {
				//GameObject* o = ResourcesManager::GetInstance()->GetPrefabManager()->GenerateGameObject(pref["prefabName"], this, nullptr);
				GameObject* o = PrefabManager::getInstance()->ParseGameObject(pref, this, nullptr);

				//Si existe un prefab con el nombre, lo rellenamos
				if (o != nullptr) {
					Add(o);
					cout << "Loaded prefab: " << pref["prefab"] << " succesfully" << endl << endl;
				}
			}
	}

	if (sceneFile.contains("Skybox")) {
		json skyObj = sceneFile["Skybox"];
		mSceneManager->setSkyDome(true, sceneFile["SkyboxPath"]);
	}
	if (sceneFile.contains("Ambient Light")) {
		json ambLight = sceneFile["Ambient Light"];
		float r, g, b;
		r = g = b = 0;
		if (ambLight.contains("r"))
			r = ambLight["r"];
		if (ambLight.contains("g"))
			g = ambLight["g"];
		if (ambLight.contains("b"))
			b = ambLight["b"];
		mSceneManager->setAmbientLight(ColourValue(r, g, b));
	}
#ifdef C_DEBUG
	cout << "==================================================\n\n";
#endif
}

void Scene::SetActive(bool active)
{
	// Activar o desactivar componentes
	for (Component* c : components) {
		if (c->isEnabled() && !active) {
			c->SetEnabled(false);
		}
		else if (!c->isEnabled() && active)
		{
			c->SetEnabled(true);
		}
	}
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

void Scene::Update() 
{
	
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

	PhysicsManager::Instance()->Update();
	//phyManager->LateUpdate();

	//Finalmente limpiamos todos los objetos pendientes de borrar
	ClearTrash();
}

void Scene::Add(GameObject * o)
{
	//A�adimos el obeto a la escena
	auto it = gameObjects.begin();
	while (it != gameObjects.end() && (*it) != o)
		it++;
	if (it != gameObjects.end()) {
#ifdef C_DEBUG
		cout << "WARNING: El objeto [ " << o->getName() << " ] ya esta en la escena" << endl;
#endif
	}
	else
		gameObjects.push_back(o);
	////Y tambien sus componentes
	//for (Component* c : o->getComponents())
	//	components.push_back(c);
}

void Scene::Add(Component * c)
{
	components.push_back(c);
}

GameObject * Scene::GenerateEmptyGameObject(string name, GameObject * parent, Vector3 position, float scale)
{
	GameObject* o = new GameObject(this, name, true, parent);
	Add(o);
	o->setPosition(position);
	o->setScale(scale);
	return o;
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
				c->setStarted();
			}
		}

		return o;
	}
}

GameObject* Scene::Instantiate(string prefab, Vector3 position, float scale, GameObject * parent)
{
	if (!destroying) {
		GameObject* o = PrefabManager::getInstance()->Instantiate(prefab, this, parent, position, scale);
		o = Instantiate(o, position, scale, parent);
		return o;
	}
	else {
#ifdef C_DEBUG
		cout << "WARNING: Intentando Instanciar en el bucle de limpieza" << endl;
#endif
		return nullptr;
	}
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
	auto it = gameObjects.begin();
	while (it != gameObjects.end() && (*it)->getName() != name)
		it++;
	bool found = it != gameObjects.end();
	if (!found) {
#ifdef C_DEBUG
		cout << "ERROR: No se ha encontrado el objeto [ " << name << " ]" << endl;
#endif
	}
	return found ? *it : nullptr;
}

void Scene::ClearTrash()
{
	//Vamos a vaciar toda la basura generada en el ciclo principal
	while (!trash.empty()) {
		GameObject* o = trash.front(); trash.pop();
#ifdef C_DEBUG
		cout << endl << "Destruyendo objeto [ "<< o->getName() <<" ]" << endl;
#endif
		//Primero le vaciamos los componentes al objeto. 
		for (Component* c : o->getComponents()) {
#ifdef C_DEBUG
			cout << "Destruyendo componente [ " << c->GetName() << " ]" << endl;
#endif
			c->OnDestroy();
			o->RemoveComponent(c);
			//Luego los quitamos de la escena
			//Y finalmente lo borramos
			components.remove(c);
			delete	c;
#ifdef C_DEBUG
			cout << "Destruido componente con exito " << endl;
#endif
		}
		//Una vez vaciado el objeto, lo quitamos de la escena y finalmente lo borramos
		mSceneManager->destroySceneNode(o->getNode());
		gameObjects.remove(o);
		delete o;
#ifdef C_DEBUG
		cout << "Destruido objeto con exito" << endl << endl;
#endif
	}
}
