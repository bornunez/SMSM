#include "Game.h"
#include "Scene.h"


Game::Game() : mRoot(0), mResourcesCfg(Ogre::BLANKSTRING), mPluginsCfg(Ogre::BLANKSTRING)
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

	//Cargar recursos
	SetUpResources();

	InitWindow();

	// Inicializar recursos
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	
	//ESCENA DE PRUEBA
	testScene = new Scene(this);

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
	while (!endGame) {
		MessagePump();
		mWindow->update(); 
		mRoot->renderOneFrame();
		testScene->Update();			// Actualiza la escena de prueba
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
