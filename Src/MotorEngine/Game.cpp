#include "Game.h"
#include "Scene.h"
#include "./Loaders/PrefabManager.h"
#include "./Loaders/ComponentLoader.h"
#include "TimeManager.h"
#include "GameSceneManager.h"
#include "AudioManager.h"


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
}

// Bucle del juego
void Game::Play() 
{
	PrefabManager::getInstance()->LoadAllPrefabs();
	//Inicializacion del audio
	AudioManager::getInstance()->init();
	// Scenes
	sceneManager->LoadScene("mainScene"); // Load the scene
	sceneManager->ChangeScene("mainScene"); // Set it to active (makes this the current active scene)

	// Another scene to test changing between scenes
	//sceneManager->LoadScene("secondScene"); // Load the scene
	//sceneManager->ChangeScene("secondScene"); // Set it to active (makes this the current active scene)

	while (!endGame) {
		MessagePump();
		mWindow->update(); 
		TimeManager::getInstance()->Update();
		//printf(" PRE RENDER");
		mRoot->renderOneFrame();

		// Current scene update
		if (sceneManager->GetActiveScene() != nullptr) {
			if (!sceneManager->GetActiveScene()->IsStarted())
				sceneManager->GetActiveScene()->Start();
			sceneManager->GetActiveScene()->Update();
		}

		//cout << mInputM->getMouseX() << " " << mInputM->getMouseY() << std::endl;
		
		if (mInputM->getMouseButtonDown(OIS::MouseButtonID::MB_Left)) std::cout << "Pulsado raton" << std::endl;
		else if (mInputM->getMouseButton(OIS::MouseButtonID::MB_Left)) std::cout << "Mantenido raton" << std::endl;
		else if (mInputM->getMouseButtonUp(OIS::MouseButtonID::MB_Left)) std::cout << "Levantado raton" << std::endl;

		if (mInputM->getKeyDown(OIS::KeyCode::KC_A)) std::cout << "Pulsado A" << std::endl;
		if (mInputM->getKey(OIS::KeyCode::KC_A)) std::cout << "Mantenido A" << std::endl;
		if (mInputM->getKeyUp(OIS::KeyCode::KC_A)) std::cout << "Levantado A" << std::endl;
		
	}
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
