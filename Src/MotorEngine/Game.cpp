#include "Game.h"
#include "Scene.h"
#include "./Loaders/PrefabManager.h"
#include "./Loaders/ComponentLoader.h"
#include "TimeManager.h"
#include "GameSceneManager.h"
#include "AudioManager.h"
#include "GUIManager.h"


Game::Game(ComponentLoader* _componentLoader) : mRoot(0), mResourcesCfg(Ogre::BLANKSTRING), mPluginsCfg(Ogre::BLANKSTRING), componentLoader(_componentLoader)
{

#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	//Crear root
	mRoot = new Ogre::Root(mPluginsCfg);
	//mRoot->loadPlugin("Codec_STBI_d");

	//Cargar recursos
	SetUpResources();

	InitWindow();

	viewport = mWindow->addViewport(nullptr);

	// Inicializar recursos
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


	//ESCENA DE PRUEBA
	//testScene = new Scene(this,"Assets/scenes/mainScene.json");

	InputManager::CreateInstance(mWindow);
	mInputM = InputManager::getInstance();
	mRoot->addFrameListener(mInputM);
}

void Game::SetUpResources()
{
	//	Carga las rutas de los recursos a partir del archivo de configuracion			
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, type, sec;

	Ogre::ConfigFile::SettingsBySection_::const_iterator secIt;
	for (secIt = cf.getSettingsBySection().begin(); secIt != cf.getSettingsBySection().end(); ++secIt) {
		sec = secIt->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = secIt->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			name = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, sec);
		}
	}

	resourcesManager = ResourcesManager::GetInstance();

	/*
		CARGAR EN EL BUFFER LOS PREFABS
	*/
	PrefabManager::getInstance()->Init("Assets/prefabs/",componentLoader);

	sceneManager = new GameSceneManager(this, "Assets/scenes/");
	sceneManager->Init();

	/*LocalizationManager locManager = *resourcesManager->GetLocalizationManager();

	locManager.Init("./Assets/");

	locManager.RegisterLanguage("ES", "_es");
	locManager.RegisterLanguage("EN", "_en");
	locManager.SelectLanguage("ES");

	locManager.RegisterFile("main_menu");

	locManager.LoadFromFiles();

	std::cout << "\n---------- Localization Testing ----------" << endl;

	std::cout << locManager.GetString("button_play") << endl;
	std::cout << locManager.GetString("button_exit") << endl;

	locManager.SelectLanguage("EN");

	std::cout << locManager.GetString("button_play") << endl;
	std::cout << locManager.GetString("button_exit") << endl;

	std::cout << "------------------------------------------" << endl;
	*/
}

void Game::InitWindow() 
{
	// Configurar render system
	/*if (!(mRoot->restoreConfig() || mRoot->showConfigDialog(NULL)))
		return false;*/

	RenderSystem* rs = *mRoot->getAvailableRenderers().begin();

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	//Crear ventana
	mWindow = mRoot->initialise(true, "SMSM");

	//Comentado porque no funciona correctamente
#ifdef NDEBUG
	GUIManager::Instance(mWindow, this);
	GUIManager::Instance()->Initialize();
#endif
}


// Bucle del juego
void Game::Play() 
{
	PrefabManager::getInstance()->LoadAllPrefabs();
	//Inicializacion del audio
	AudioManager::getInstance()->init();
	//// Scenes
	//sceneManager->LoadScene("menuScene"); // Load the scene
	//sceneManager->ChangeScene("menuScene"); // Set it to active (makes this the current active scene)
	//										// Scenes
#ifdef NDEBUG
	sceneManager->LoadScene("menuScene"); // Load the scene
	sceneManager->ChangeScene("menuScene"); // Set it to active (makes this the current active scene)
#else
	sceneManager->LoadScene("mainScene"); // Load the scene
	sceneManager->ChangeScene("mainScene"); // Set it to active (makes this the current active scene)
#endif
	// Another scene to test changing between scenes
	//sceneManager->LoadScene("secondScene"); // Load the scene
	//sceneManager->ChangeScene("secondScene"); // Set it to active (makes this the current active scene)

	while (!endGame) {
		MessagePump();
		
#ifdef NDEBUG
		GUIManager::Instance()->Update();
#endif
		mRoot->renderOneFrame();

		//if (GUIManager::Instance()->getGameOn()) {
			mWindow->update();

			TimeManager::getInstance()->Update();
			
			//printf(" PRE RENDER");
			mRoot->renderOneFrame();

			// Current scene update
			if (sceneManager->GetActiveScene() != nullptr) {
				if (!sceneManager->GetActiveScene()->IsStarted()) {
					sceneManager->GetActiveScene()->Start();
					TimeManager::getInstance()->setDeltaTime(0);
				}
				sceneManager->GetActiveScene()->Update();
			}
		//}

		//if (mInputM->getKeyUp(OIS::KeyCode::KC_ESCAPE)) endGame = true;
	}
}

void Game::Quit()
{
	endGame = true;
}

void Game::ReLoadScene(std::string scene)
{
	sceneManager->RealoadScene(scene);		// Load the scene
	sceneManager->ChangeScene(scene);	// Set it to active (makes this the current active scene)
}

// Permite actuar con el raton sobre la ventana
void Game::MessagePump()
{
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


Game::~Game()
{
	delete mRoot;
}
