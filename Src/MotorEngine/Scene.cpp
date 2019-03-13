#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include "../../Projects/MotorEngine/Movement.h"
#include "AudioManager.h"

Scene::Scene(Game* _g) : g(_g)
{

	// CREACION DE ESCENA DE PRUEBA

	// Crear SceneManager
	mSceneManager = g->getRoot()->createSceneManager();

	// Crea el nodo de la escena como hijo de root
	sceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	phyManager = new PhysicsManager();

	//Crea un nodo de escena, hijo de root
	//mSceneManager->getRootSceneNode()->createChildSceneNode();

	// Crear la camara
	Camera* cam = mSceneManager->createCamera("Cam");
	cam->setNearClipDistance(6);



	mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(cam);

	mCamNode->setPosition(0, 0, 80);
	mCamNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_WORLD);

	// Crear ViewPort
	Ogre::Viewport* vp = g->getRenderWindow()->addViewport(cam);

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	//// Escena
	Ogre::Entity* ogreEntity = mSceneManager->createEntity("ogrehead.mesh");

	/*testNode = sceneNode->createChildSceneNode();
	testNode->attachObject(ogreEntity);
	testNode->setScale(0.1, 0.1, 0.1);*/

	////Cada nodo acaba teniendo una entidad y un rigidbody asociado, que puede colocar en un grupo o en otro para poder realizar acciones sobre un conjunto
	////de rigidbodies al mismo tiempo
	//
	//btRigidBody* rb =  phyManager->CreateSphereCollider(testNode, 100, btVector3(0, 20, 0), btQuaternion(), 0.8f, 2);
	//phyManager->AddRigidBodyToGroup(rb, 0);


	//SECOND HEAD
	//Ogre::Entity* ogreEntity2 = mSceneManager->createEntity("cube.mesh");

	//secondNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	//secondNode->attachObject(ogreEntity2);
	//secondNode->setScale(0.05, 0.05, 0.05);

	btRigidBody* rb1 = phyManager->CreateBoxCollider(secondNode, 0, btVector3(3, 0, 0), btQuaternion(), 0.5, btVector3(3,3,3));
	phyManager->AddRigidBodyToGroup(rb1, 1);

	// Crear luz

	Light* luz = mSceneManager->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);

	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));  //vec3.normalise();

	AudioManager* audioManager = new AudioManager();
	audioManager->playSound("CorazonPartio", false, 1, CHANNEL::Default);


}


void Scene::Load()
{
	/*
	CARGA DE ESCENA POR FICHEROS / PONER LOS OBJETOS A MANO
	*/
	GameObject* o1 = new GameObject(this, mSceneManager,"Head");
	o1->AddMesh("ogrehead.mesh");
	Add(o1);
	Movement* m = new Movement(o1);
	o1->AddComponent(m);
	Add(m);
	o1->getNode()->setScale(0.25, 0.25, 0.25);



	GameObject* child = new GameObject(this, mSceneManager, "Child");
	child->setPosition({ 0,-30,0 });
	child->AddMesh("cube.mesh");
	child->getNode()->setScale(0.25, 0.25, 0.25);
	o1->AddChild(child);

	for (Component* c : components) {
		c->Awake();
	}
}

void Scene::Start()
{
	for (Component* c : components) {
		c->Start();
	}
}

// Mueve el cubo a un lado en x.
void Scene::Update() 
{
	phyManager->Update();

	//testNode->setPosition(testNode->getPosition().x + 1,testNode->getPosition().y, testNode->getPosition().z);

	//phyManager->AddDiscreteImpulse(btVector3(1, 1, 0), 20, 0);

	//Recorremos los updates de los objetos activos
	//cout << "Nº Componentes: " << components.size() << endl;
	for (Component* c : components) {
		/*cout << "Active: " << c->getGameObject()->isActive() << endl;
		cout << "Enabled: " << c->Enabled() << endl;*/
		if (c->isActiveAndEnabled()) c->Update();
	}
	//Tambien el late update de los objs activos
	for (Component* c : components) {
		if (c->isActiveAndEnabled()) c->LateUpdate();
	}
	//Finalmente limpiamos todos los objetos pendientes de borrar
	ClearTrash();
}

void Scene::Add(GameObject * o)
{
	//Añadimos el obeto a la escena
	gameObjects.push_back(o);
	//Y tambien sus componentes
	for (Component* c : o->getComponents())
		components.push_back(c);
}

void Scene::Add(Component * c)
{
	components.push_back(c);
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
		gameObjects.remove(o);
		delete o;
	}
}











Scene::~Scene()
{
}
