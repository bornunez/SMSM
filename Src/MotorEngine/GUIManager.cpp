#include "GUIManager.h"
#include "Game.h"
#include "Ogre.h"
#include "OgreRectangle2D.h"

GUIManager* GUIManager::instance_ = nullptr;

GUIManager::GUIManager(Ogre::RenderWindow* w, Game* g)
{
	renderWindow_ = w;
	g_ = g;

	// Registramos las funciones de los botones con un nombre distintivo en un diccionario de funciones
	functions["exit"]			= &GUIManager::Exit;
	functions["mainScene"]		= &GUIManager::InitMainScene;
	functions["pause"]			= &GUIManager::togglePause;
	functions["menu"]			= &GUIManager::toggleMenu;
}

GUIManager::~GUIManager()
{
	// toDo: release memory

	CEGUI::OgreRenderer::destroySystem();
}

GUIManager * GUIManager::Instance(Ogre::RenderWindow* w, Game* g)
{
	// Para no sobreescribir una instancia del GUIManager
	if (instance_ == nullptr) {
		instance_ = new GUIManager(w, g);
	}
	return instance_;
}

void GUIManager::Update()
{
	checkKeys();
	checkMouse();	
}

void GUIManager::checkMouse()
{
	// Los inject permiten comprobar la posicion del mouse en el hud y cuando se hace click sobre las diferentes ventanas (botones)

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// Actualizamos la posicion del raton
	context.injectMousePosition(InputManager::getInstance()->getMouseX(), InputManager::getInstance()->getMouseY());

	// Click izquierdo
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Left)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::LeftButton);
	}
	else if (!gameEnded && InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Left)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::LeftButton);
	}

	// Click rueda
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Middle)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::MiddleButton);
	}
	else if (InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Middle)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::MiddleButton);
	}

	// Click derecho
	if (InputManager::getInstance()->getMouseButtonDown(OIS::MouseButtonID::MB_Right)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MouseButton::RightButton);
	}
	else if (InputManager::getInstance()->getMouseButtonUp(OIS::MouseButtonID::MB_Right)) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MouseButton::RightButton);
	}
}

void GUIManager::checkKeys()
{
	if (gameHUD) {
		if (InputManager::getInstance()->getKeyDown(OIS::KeyCode::KC_ESCAPE)) {		//ESCAPE es el unico boton de teclado que queremos reconocer en los menus (para entrar o salir de la pausa)
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);			
		}
		else if (InputManager::getInstance()->getKeyUp(OIS::KeyCode::KC_ESCAPE)) {
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)OIS::KeyCode::KC_ESCAPE);
			togglePause();
		}
	}
}

void GUIManager::Initialize()
{
	// Init CEGUI System
	renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(renderWindow_));

	// Load CEGUI Resources
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	// Set default resources
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");	

	// Create CEGUI root object
	wmgr = &CEGUI::WindowManager::getSingleton();
	myRoot = wmgr->createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	// Todo esto debe cargarse desde un archivo que modificamos desde un archivo

	//Se deberia llamar desde una entidad de la escena menu
	//FrameWndImage("fondomenu");

	////Main menu window
	// COMMENT EVENTUALLY --------------------------------------
	//{
	//	menuWnd = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow("TaharezLook/FrameWindow", "menuWindow"));
	//	myRoot->addChild(menuWnd);
	//	menuWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.2f, -0.2f), CEGUI::UDim(-0.2f, -0.2f)));
	//	menuWnd->setSize(CEGUI::USize(CEGUI::UDim(1.3f, 1.3f), CEGUI::UDim(1.3f, 1.3f)));
	//	menuWnd->setTitleBarEnabled(false);
	//	menuWnd->setCloseButtonEnabled(false);

	//	//Main menu buttons
	//	{
	//		CEGUI::Window *quit = wmgr->createWindow("TaharezLook/Button");
	//		menuWnd->addChild(quit);
	//		quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
	//		quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	//		quit->setText("Quit");
	//		quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions["exit"], this));

	//		CEGUI::Window *start = wmgr->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	//		menuWnd->addChild(start);
	//		start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	//		start->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	//		start->setText("Start");
	//		start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions["mainScene"], this));
	//	}

	//	stateWnds["MenuWnd"] = menuWnd;
	//	activeWnd = menuWnd;
	//}

	////Pause menu window
	//{
	//	pauseWnd = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow("TaharezLook/FrameWindow", "pauseWindow"));
	//	myRoot->addChild(pauseWnd);
	//	pauseWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0.2f), CEGUI::UDim(0.2f, 0.2f)));
	//	pauseWnd->setSize(CEGUI::USize(CEGUI::UDim(0.6f, 0.6f), CEGUI::UDim(0.6f, 0.6f)));
	//	//pauseWnd->setTitleBarEnabled(false);
	//	pauseWnd->setDragMovingEnabled(false);
	//	pauseWnd->setCloseButtonEnabled(false);
	//	pauseWnd->setSizingEnabled(false);

	//	//Pause menu buttons
	//	{
	//		CEGUI::Window *quit = wmgr->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	//		pauseWnd->addChild(quit);
	//		quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0.0f), CEGUI::UDim(0.5f, 0.0f)));
	//		quit->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	//		quit->setText("Menu");
	//		quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions["pause"], this));
	//		quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions["menu"], this));

	//		CEGUI::Window *start = wmgr->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	//		pauseWnd->addChild(start);
	//		start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	//		start->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
	//		start->setText("Continue");
	//		start->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions["pause"], this));
	//		start->setDisabled(true);
	//	}
	//	stateWnds["pauseMenu"] = pauseWnd;
	//	pauseWnd->hide();
	//}
}

// NOTA:

// El problema con la asignacion de los callbacks de los botones es la asignacion de parametros
// Es decir se puede hacer con diccionario de metodos sin parametros que llamen por debajo a metodos con
// parametros espeficicos para ese metodo

// Hay que pasar el nombre con el que esta mapeado el metodo dentro de la lista "functions"

CEGUI::Window*  GUIManager::CreateButton(std::string stateWnd, std::string buttonName, std::string buttonScheme, float pos_x, float pos_y, float size_x, float size_y, std::string text, std::string methodName)
{
	CEGUI::Window *temp = wmgr->createWindow(buttonScheme, buttonName);

	if(stateWnd != "null")
		stateWnds[stateWnd]->addChild(temp);

	temp->setPosition(CEGUI::UVector2(CEGUI::UDim(pos_x, 0.0f), CEGUI::UDim(pos_y, 0.0f)));
	temp->setSize(CEGUI::USize(CEGUI::UDim(size_x, 0), CEGUI::UDim(size_y, 0)));
	temp->setText(text);

	if(methodName != "null")
		temp->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(functions[methodName], this));

	return temp;
}

CEGUI::FrameWindow * GUIManager::AddWindow(std::string wndName, std::string frameWindowLook, int posX, int posy, int sizeX, int sizeY, std::string backgroundMatName)
{
	if (stateWnds[wndName] == nullptr) {	// Si esa ventana no se ha creado ya
		stateWnds[wndName] = static_cast<CEGUI::FrameWindow*>(wmgr->createWindow(frameWindowLook, wndName));	//
		myRoot->addChild(stateWnds[wndName]);
		stateWnds[wndName]->setPosition(CEGUI::UVector2(CEGUI::UDim(-0.2f, -0.2f), CEGUI::UDim(-0.2f, -0.2f)));
		stateWnds[wndName]->setSize(CEGUI::USize(CEGUI::UDim(1.3f, 1.3f), CEGUI::UDim(1.3f, 1.3f)));
		stateWnds[wndName]->setTitleBarEnabled(false);
		stateWnds[wndName]->setCloseButtonEnabled(false);
		stateWnds[wndName]->setSizingEnabled(false);


		// Si el material está definido
		/*if (backgroundMatName != " ") {
			FrameWndImage(backgroundMatName);
		}*/
	}

	return stateWnds[wndName];
}

// Metodo para poder utilizar una imagen como fondo de una ventana de menu
void GUIManager::FrameWndImage(std::string name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real down)
{
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name, "General");
	material->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);

	rect->setCorners(left, top, right, down);
	rect->setMaterial(material);

	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	Ogre::SceneNode* node = g_->getRoot()->createSceneManager()->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(rect);
}


void GUIManager::Exit()
{
	if (g_ != nullptr) {
		gameEnded = true;
		g_->Quit();
	}
}

void GUIManager::InitMainScene()
{
	if (g_ != nullptr) {
		// Esto se comentará porque no hace falta al llamarse en el disable
		HideWindow("MenuWnd");
		gameHUD = true;
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		g_->ReLoadScene("mainScene");
	}
}

void GUIManager::ToggleWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {

		if (stateWnds[wndName] == activeWnd) {	//Si es la que está activa, lo que queremos es apagarla y poner la default (el HUD del juego)
			activeWnd->hide();
			activeWnd = lastWnd;
			activeWnd->show();
		}
		else {
			if (activeWnd != nullptr)
				activeWnd->hide();					//Apagamos la ventana que estuviera activa	(Activar segun parametro de si comienza activa o no)	
			lastWnd = activeWnd;					//Nos guardamos la ultima
			activeWnd = stateWnds[wndName];			//Ponemos la nueva como la activa
			activeWnd->show();						//Activamos la nueva
		}
	}
}

void GUIManager::HideWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {
		stateWnds[wndName]->hide();
	}
	else {
		std::cout << "No se encuentra la ventana en el diccionario" << std::endl;
	}
}

void GUIManager::ShowWindow(std::string wndName)
{
	if (stateWnds[wndName] != nullptr) {
		stateWnds[wndName]->show();
	}
	else {
		std::cout << "No se encuentra la ventana en el diccionario" << std::endl;
	}
}

void GUIManager::toggleMenu()
{
	if (menuHUD) {
		HideWindow("MenuWnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		gameHUD = true;
	}
	else {
		ShowWindow("MenuWnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
		gameHUD = false;
	}

	menuHUD = !menuHUD;
}

void GUIManager::togglePause()
{	
	if (pauseHUD) {
		HideWindow("PauseWnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		gameHUD = true;
	}
	else {
		ShowWindow("PauseWnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
		gameHUD = false;
	}

	pauseHUD = !pauseHUD;
}